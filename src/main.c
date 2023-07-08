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
#include "drivers/sharp.h"
#include "drivers/usb/usb_serial.h"

#include "utils.h"

#define LED 13


enum  {
  BLINK_NEVER_MOUNTED = 250,
  BLINK_MOUNTED = 1000,
  BLINK_SUSPENDED = 2500,
  BLINK_UNPLUGGED = 100,
};

static uint32_t blink_interval_ms = BLINK_NEVER_MOUNTED;


void led_blinking_task(void);


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

    // wait for USB to come up
    while(!tud_ready()) {
        tud_task();
    }
    wait_for_serial_hack();
    
    printf("start\n");

    // can't let main return otherwise it breaks core1
    while (true) {
        led_blinking_task();

        tud_task(); // tinyusb device task
    }

    return 0;
}


//--------------------------------------------------------------------+
// Device callbacks
//--------------------------------------------------------------------+

// Invoked when device is mounted
void tud_mount_cb(void)
{
  blink_interval_ms = BLINK_MOUNTED;
}

// Invoked when device is unmounted
void tud_umount_cb(void)
{
  blink_interval_ms = BLINK_UNPLUGGED;
}

// Invoked when usb bus is suspended
// remote_wakeup_en : if host allow us  to perform remote wakeup
// Within 7ms, device must draw an average of current less than 2.5 mA from bus
void tud_suspend_cb(bool remote_wakeup_en)
{
  (void) remote_wakeup_en;
  blink_interval_ms = BLINK_SUSPENDED;
}

// Invoked when usb bus is resumed
void tud_resume_cb(void)
{
  blink_interval_ms = BLINK_MOUNTED;
}


//--------------------------------------------------------------------+
// BLINKING TASK
//--------------------------------------------------------------------+
void led_blinking_task(void)
{
  static uint32_t start_ms = 0;
  static bool led_state = false;

  // Blink every interval ms
  if ( board_millis() - start_ms < blink_interval_ms) return; // not enough time
  start_ms += blink_interval_ms;


  printf("writing %d\n", led_state);

  gpio_put(LED, led_state);
  led_state = 1 - led_state; // toggle
}