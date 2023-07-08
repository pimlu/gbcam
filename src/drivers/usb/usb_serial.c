#include "usb_serial.h"

#include "pico/stdlib.h"

#include "bsp/board.h"
#include "tusb.h"

// idk why this is necessary, but it seems sleeping by itself is not enough
void wait_for_serial_hack() {
    for (int i=0; i<500; i++) {
        tud_task();
        sleep_ms(1);
    }
}