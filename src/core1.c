#include "core1.h"


#include <stdio.h>
#include <string.h> 
#include "pico/stdlib.h"

#include "utils.h"
#include "drivers/gbcam.h"


// 16KB, kind of brutal but we have way more than that
static uint8_t cameraData[GBCAM_H * GBCAM_W];

uint16_t avgBrightness() {
    uint32_t sum = 0;
    for (uint i = 0; i < GBCAM_H * GBCAM_W; i++) {
        // penalize inverted greyness by squaring the brightness
        sum += cameraData[i] * (uint32_t) cameraData[i];
    }
    return sum >> (7 + 7 + 3);
}

// above and below this but within the bounds, we nudge just a little
#define TARGET_BRIGHTNESS 0x225

#define MIN_EXPOSURE 0x0100
#define MAX_EXPOSURE 0xffff

void core1_main() {
    printf("core1 init\n");

    gbcam_init();

    uint32_t exposure = 0x0100;

    while (true) {
        while (sharedBuffer.phase != SNAP_READY) busy_wait_at_least_cycles(99);
        sharedBuffer.phase = SNAPPING;

        // printf("core1 snap")

        gbcam_snap(cameraData, (uint16_t) exposure);
        // printf("core1 done\n");
        memcpy(sharedBuffer.data, cameraData, sizeof sharedBuffer.data);
        // break;
        sharedBuffer.phase = SNAPPED;

        uint16_t avg = avgBrightness();

        int16_t diff = TARGET_BRIGHTNESS - (int16_t) avg;
        int16_t mag = diff >= 0 ? diff : -diff;

        // hand tuned white balance that essentially says, the further we
        // are away from the target brightness, the more we should adjust
        #define X 20
        int16_t nudge;
        if      (mag > 35*X) nudge = 24;
        else if (mag > 25*X) nudge = 16;
        else if (mag > 15*X) nudge = 12;
        else if (mag > 10*X) nudge = 7;
        else if (mag > 3*X)  nudge = 3;
        else if (mag > X)    nudge = 1;
        else                 nudge = 0;

        if (diff < 0) nudge = -nudge;

        printf("core1 snapped exp=0x%04lx avg=0x%04x nudge=%d\n", exposure, avg, nudge);

        // multiply the exposure by 1 + nudge/64
        exposure = (exposure * (64 + nudge)) >> 6;

        // clamp exposure
        if (exposure < MIN_EXPOSURE) exposure = MIN_EXPOSURE;
        else if (exposure > MAX_EXPOSURE) exposure = MAX_EXPOSURE;
    }

    // dump(cameraData, GBCAM_H * GBCAM_W);
    // printf("dumped\n");

    while (true) tight_loop_contents();
}