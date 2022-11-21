/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"

int LED = 13;
int main() {
    stdio_init_all();
    gpio_init(LED);
    gpio_set_dir(LED, GPIO_OUT);

    while (true) {
        printf("Hello, world!\n");
        sleep_ms(750);
        gpio_put(LED, 1);
        sleep_ms(250);
        gpio_put(LED, 0);
    }
    return 0;
}
