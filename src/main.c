/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/spi.h"
#include "hardware/adc.h"
#include "pico/multicore.h"

#include "drivers/gbcam.h"
#include "drivers/sharp.h"

#include "utils.h"

#define LED 13


// 16KB, kind of brutal but we have way more than that
static uint8_t cameraData[GBCAM_H * GBCAM_W];

void camera_snap() {
    printf("init\n");
    gbcam_init();
    printf("snap\n");

    gbcam_snap(cameraData);
    printf("done\n");
    dump(cameraData, GBCAM_H * GBCAM_W);
    printf("dumped\n");

    while (true) {
    }
}

int main() {
    stdio_init_all();
    
    adc_init();

    gpio_init(LED);
    gpio_set_dir(LED, GPIO_OUT);

    // spi_sharp_init();

    // bool vcom = false;
    // while (true) {
    //     draw(vcom);

    //     sleep_ms(750);
    //     gpio_put(LED, 1);
    //     sleep_ms(250);
    //     gpio_put(LED, 0);

    //     vcom = !vcom;
    // }

    // to wait for serial to come up
    sleep_ms(1000);

    // snap the camera on core1 since core0 is busy with USB/serial interrupts
    multicore_launch_core1(camera_snap);

    // can't let main return otherwise it breaks core1
    while (true) {
        sleep_ms(1000);
    }

    return 0;
}
