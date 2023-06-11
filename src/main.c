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

#include "drivers/gbcam.h"
#include "drivers/sharp.h"

#define LED 13

int main() {
    stdio_init_all();
    
    adc_init();

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
