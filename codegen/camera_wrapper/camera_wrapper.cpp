#include "camera_wrapper.h"

#include <libcamera/libcamera.h>
#include <libcamera/formats.h>
#include <sys/mman.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <mutex>
#include <atomic>
#include <vector>
#include <map>
#include <algorithm>

using namespace libcamera;
using namespace std::chrono_literals;

// --- Global Variables ---
static std::shared_ptr<Camera> camera;
static std::unique_ptr<CameraManager> cm;
static std::unique_ptr<FrameBufferAllocator> allocator;
static std::vector<std::unique_ptr<Request>> requests;

// CRITICAL: Config must stay alive globally
static std::unique_ptr<CameraConfiguration> config; 
static StreamConfiguration *streamConfig = nullptr;
static Stream *stream = nullptr;

static std::vector<uint8_t> lastImage;
static std::atomic<bool> running = false;

struct MappedPlane {
    void *data;
    size_t length;
};
static std::map<FrameBuffer *, std::vector<MappedPlane>> memory_cache;

static PixelFormat getForcedFormat() {
    //return formats::XRGB8888;
    return formats::BGR888;
}

/*
static void requestComplete(Request *request) {
    if (request->status() == Request::RequestCancelled) return;

    const std::map<const Stream *, FrameBuffer *> &buffers = request->buffers();
    auto it = buffers.find(stream);
    if (it == buffers.end()) return;

    FrameBuffer *buffer = it->second;

    auto plane_it = memory_cache.find(buffer);
    if (plane_it == memory_cache.end()) return;
    
    void *data = plane_it->second[0].data;
    if (!data) return;

    const int W = streamConfig->size.width;
    const int H = streamConfig->size.height;
    const size_t stride = streamConfig->stride;
    
    std::lock_guard<std::mutex> lock(imageMutex);
    
    if (lastImage.size() != (size_t)W * H * 3) {
        lastImage.resize(W * H * 3);
    }
    
    // Pointers to the Planar Channels (Simulink Format)
    uint8_t *dstR = lastImage.data();
    uint8_t *dstG = dstR + (W * H);
    uint8_t *dstB = dstG + (W * H);
    
    const uint8_t *src = static_cast<uint8_t*>(data);

    // Loop: Input is Row-Major XRGB, Output must be Column-Major RGB
    for (int y = 0; y < H; ++y) {
        const uint8_t *src_row = src + (y * stride);
        
        for (int x = 0; x < W; ++x) {
            // Calculate Simulink Column-Major Index: row + col * Height
            int dst_idx = y + (x * H);

            // Read XRGB (B G R X)
            uint8_t b = src_row[x * 4 + 0];
            uint8_t g = src_row[x * 4 + 1];
            uint8_t r = src_row[x * 4 + 2];

            // Write to separate planes
            dstR[dst_idx] = r;
            dstG[dst_idx] = g;
            dstB[dst_idx] = b;
        }
    }

    if (running) {
        request->reuse(Request::ReuseBuffers);
        camera->queueRequest(request);
    }
}*/

static uint8_t image_buffer[307200]; 

// 2. Atomic Flag (Lock-free signaling)
static std::atomic<bool> is_fresh(false);
static std::mutex imageMutex; 

// ... (keep includes and other globals) ...

static void requestComplete(Request *request) {
    if (request->status() == Request::RequestCancelled) return;

    const std::map<const Stream *, FrameBuffer *> &buffers = request->buffers();
    auto it = buffers.find(stream);
    if (it == buffers.end()) return;

    auto plane_it = memory_cache.find(it->second);
    if (plane_it == memory_cache.end()) return;
    
    void *data = plane_it->second[0].data;
    if (!data) return;

    // Fixed size copy - No "resize" check needed
    // We assume the camera is configured to 320x320 BGR888 correctly
    {
        std::lock_guard<std::mutex> lock(imageMutex);
        std::memcpy(image_buffer, data, 307200);
        is_fresh = true;
    }

    if (running) {
        request->reuse(Request::ReuseBuffers);
        camera->queueRequest(request);
    }
}

bool camera_wrapper_init(const CameraConfig *cfg) {
    if (running) return true;

    cm = std::make_unique<CameraManager>();
    if (cm->start()) {
        std::cerr << "CameraManager start failed\n";
        return false;
    }

    if (cm->cameras().empty()) {
        std::cerr << "No cameras found\n";
        return false;
    }

    std::string id = (cfg->cameraId && cfg->cameraId[0] != '\0') 
                     ? cfg->cameraId 
                     : cm->cameras()[0]->id();
    
    camera = cm->get(id);
    if (!camera) {
        std::cerr << "Camera not found\n";
        return false;
    }

    if (camera->acquire()) {
        std::cerr << "Camera acquire failed\n";
        return false;
    }

    config = camera->generateConfiguration({ StreamRole::Viewfinder });
    streamConfig = &config->at(0);
    streamConfig->size.width = cfg->width;
    streamConfig->size.height = cfg->height;
    streamConfig->pixelFormat = getForcedFormat();

    config->validate();
    camera->configure(config.get());

    std::cout << "Camera Configured: " << streamConfig->toString() 
              << " | Stride: " << streamConfig->stride << std::endl;

    allocator = std::make_unique<FrameBufferAllocator>(camera);
    allocator->allocate(streamConfig->stream());

    stream = streamConfig->stream();
    const auto &bufs = allocator->buffers(stream);

    memory_cache.clear();
    for (const std::unique_ptr<FrameBuffer> &buffer : bufs) {
        std::vector<MappedPlane> mapped_planes;
        for (const FrameBuffer::Plane &plane : buffer->planes()) {
            int fd = plane.fd.get();
            size_t length = plane.length;
            void *data = mmap(NULL, length, PROT_READ, MAP_SHARED, fd, 0);
            mapped_planes.push_back({data, length});
        }
        memory_cache[buffer.get()] = mapped_planes;
    }

    requests.clear();
    for (unsigned int i = 0; i < bufs.size(); ++i) {
        std::unique_ptr<Request> request = camera->createRequest();
        request->addBuffer(stream, bufs[i].get());
        requests.push_back(std::move(request));
    }

    ControlList controls(camera->controls());
    int64_t frame_time = 1000000 / 30; 
    controls.set(controls::FrameDurationLimits, { frame_time, frame_time });

    camera->requestCompleted.connect(requestComplete);
    
    // CRITICAL FIX: Set running=true BEFORE starting the camera
    // This prevents the first frame from completing before we are "ready", 
    // which would cause the loop to stop immediately.
    running = true;

    if (camera->start(&controls)) {
        std::cerr << "Camera start failed\n";
        running = false;
        return false;
    }

    for (auto &req : requests)
        camera->queueRequest(req.get());

    return true;
}

void camera_wrapper_stop() {
    if (!running) return;

    running = false;
    camera->stop();
    camera->requestCompleted.disconnect(requestComplete);

    for (auto const& [buffer, planes] : memory_cache) {
        for (const auto &plane : planes) {
            if (plane.data) munmap(plane.data, plane.length);
        }
    }
    memory_cache.clear();

    if (allocator && stream) allocator->free(stream);
    allocator.reset();

    if (camera) {
        camera->release();
        camera.reset();
    }
    if (cm) {
        cm->stop();
        cm.reset();
    }
}

const uint8_t *camera_wrapper_get_latest(size_t *size) {
    std::lock_guard<std::mutex> lock(imageMutex);
    if (size) *size = 307200;
    // Return pointer to static buffer
    return image_buffer; 
}