#include "core1.h"


#include <stdio.h>
#include <string.h> 
#include "pico/stdlib.h"

#include "utils.h"
#include "drivers/gbcam.h"


// 16KB, kind of brutal but we have way more than that
static uint8_t cameraData[GBCAM_H * GBCAM_W];

void core1_main() {
    printf("core1 init\n");

    gbcam_init();

    while (true) {
        while (sharedBuffer.phase != SNAP_READY) busy_wait_at_least_cycles(99);
        sharedBuffer.phase = SNAPPING;

        printf("core1 snap\n");

        gbcam_snap(cameraData);
        printf("core1 done\n");
        memcpy(sharedBuffer.data, cameraData, sizeof sharedBuffer.data);
        // break;
        sharedBuffer.phase = SNAPPED;
    }

    // dump(cameraData, GBCAM_H * GBCAM_W);
    // printf("dumped\n");

    while (true) tight_loop_contents();
}