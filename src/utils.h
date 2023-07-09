#pragma once

#include <stdio.h>

#include "constants.h"

void dump(uint8_t* buf, size_t len);

typedef enum {
    SNAP_READY, // waiting for core1.c gbcam driver
    SNAPPING, // gbcam driver busy
    SNAPPED, // gbcam driver done, waiting for video.c to copy
} phase_t;

typedef struct {
    phase_t phase;
    uint8_t data[GBCAM_W * GBCAM_H];
} video_buffer_t;

extern video_buffer_t sharedBuffer;