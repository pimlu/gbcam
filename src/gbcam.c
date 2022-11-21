/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/spi.h"

#include "sharp.h"

#define LED 13


void dump(uint8_t* buf, size_t len) {
    for (size_t i=0; i<len; i++) {
        printf("%02x ", buf[i]);
    }
    printf("\n");
}

int main() {
    stdio_init_all();
    gpio_init(LED);
    gpio_set_dir(LED, GPIO_OUT);

    spi_sharp_init();

    bool vcom = false;
    while (true) {

        draw(vcom);

        sleep_ms(750);
        gpio_put(LED, 1);
        sleep_ms(250);
        gpio_put(LED, 0);

        vcom = !vcom;
    }


    return 0;
}
