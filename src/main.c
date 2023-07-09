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

#include "core1.h"
#include "utils.h"


int main() {
    board_init();
    tusb_init();
    stdio_usb_init();
    tud_task();
    
    adc_init();
    gpio_init(BOARD_LED);
    gpio_set_dir(BOARD_LED, GPIO_OUT);

    // wait for USB to come up
    while(!tud_ready()) {
        tud_task();
    }
    wait_for_serial_hack();

    multicore_launch_core1(core1_main);
    
    printf("core0 init\n");

    // can't let main return otherwise it breaks core1
    while (true) {
        led_blinking_task();
        video_task();

        tud_task(); // tinyusb device task
    }

    return 0;
}
