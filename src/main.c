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

#include "bsp/board.h"
#include "tusb.h"

#include "drivers/gbcam.h"
#include "drivers/usb/usb_serial.h"

#include "tasks/debug_led.h"
#include "tasks/video.h"

#include "utils.h"


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
    board_init();
    tusb_init();
    stdio_usb_init();
    tud_task();

    gpio_init(BOARD_LED);
    gpio_set_dir(BOARD_LED, GPIO_OUT);

    // wait for USB to come up
    while(!tud_ready()) {
        tud_task();
    }
    wait_for_serial_hack();
    
    printf("start\n");

    // can't let main return otherwise it breaks core1
    while (true) {
        led_blinking_task();
        video_task();

        tud_task(); // tinyusb device task
    }

    return 0;
}
