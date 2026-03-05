#ifndef CAMERA_WRAPPER_H
#define CAMERA_WRAPPER_H

#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Camera configuration struct
typedef struct {
    int deviceIndex;        // 0 = first camera
    const char *cameraId;   // optional; if non-empty, uses this cameraId
    int width;              // requested width
    int height;             // requested height
    const char *pixelFormat;// "YUYV" (will be converted to RGB888 before return)
} CameraConfig;

/**
 * Initialize the camera with given settings.
 * Returns true on success, false on failure.
 */
bool camera_wrapper_init(const CameraConfig *cfg);

/**
 * Stop and clean up the camera.
 */
void camera_wrapper_stop(void);

/**
 * Get the most recent frame in RGB888 format.
 * @param size: if not NULL, set to number of bytes in frame.
 * @return pointer to frame buffer (valid until next call).
 */
const unsigned char *camera_wrapper_get_latest(size_t *size);

#ifdef __cplusplus
}
#endif

#endif // CAMERA_WRAPPER_H
