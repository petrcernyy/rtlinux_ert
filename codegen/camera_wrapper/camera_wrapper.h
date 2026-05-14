#ifndef CAMERA_WRAPPER_H
#define CAMERA_WRAPPER_H

#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int deviceIndex;
    const char *cameraId;
    int width;
    int height;
    const char *pixelFormat;
} CameraConfig;

bool camera_wrapper_init(const CameraConfig *cfg);

void camera_wrapper_stop(void);

const unsigned char *camera_wrapper_get_latest(size_t *size);

#ifdef __cplusplus
}
#endif

#endif // CAMERA_WRAPPER_H
