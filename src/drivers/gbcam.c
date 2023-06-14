#include "gbcam.h"

#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"

#include <string.h>


// 16KB, kind of brutal but we have way more than that
// static uint8_t cameraData[GBCAM_H * GBCAM_W];

void gbcam_init() {
    gpio_init(GBCAM_START);
    gpio_set_dir(GBCAM_START, GPIO_OUT);
    gpio_init(GBCAM_SIN);
    gpio_set_dir(GBCAM_SIN, GPIO_OUT);
    gpio_init(GBCAM_LOAD);
    gpio_set_dir(GBCAM_LOAD, GPIO_OUT);
    gpio_init(GBCAM_XRST);
    gpio_set_dir(GBCAM_XRST, GPIO_OUT);
    gpio_init(GBCAM_XCK);
    gpio_set_dir(GBCAM_XCK, GPIO_OUT);
    gpio_init(GBCAM_RESET);
    gpio_set_dir(GBCAM_RESET, GPIO_OUT);

    gpio_init(GBCAM_READ);
    gpio_set_dir(GBCAM_RESET, GPIO_IN);

    adc_gpio_init(GBCAM_VOUT);
    adc_select_input(GBCAM_VOUT_ADC);  
}

// from the datasheet, example image capture mode
// "Positive image (without dark pixel output)"
static uint8_t registers[8] = { 0x80, 0x04, 0x11, 0x11, 0xa1, 0x10, 0x01, 0x03 };

#define put(pin, b) gpio_put(GBCAM_ ## XCK, b)

void send_bits (uint n, uint bits) {
    for (uint b = 0; b < bits; b++) {
        put(XCK, 1);
        put(SIN, n & 1);
        n >>= 1;
        put(XCK, 0);
    }
}

static void gbcam_reset() {
    put(XCK, 0);
    put(XRST, 0);
    put(RESET, 0);

    // diagram timing has one clock while resetting
    put(XCK, 1);
    put(XCK, 0);

    put(XRST, 1);
    put(RESET, 1);
}

static void gbcam_initregs() {
    for (uint r = 0; r < 8; r++) {
        send_bits(r, 3);
        send_bits(registers[r], 7);

        put(XCK, 1);
        put(LOAD, 1);
        // last bit has to be done separately since the LOAD pin is high
        put(SIN, registers[r] >> 7);

        put(XCK, 0);
        put(LOAD, 0);
    }
}

static void gbcam_expose() {
    put(START, 1);
    put(XCK, 1);
    // TODO okay to do this so soon?
    put(START, 0);
    put(XCK, 0);

    while (gpio_get(GBCAM_READ) == 0) {
        put(XCK, 0);
        put(XCK, 1);
    }
}

static void gbcam_readsignal(uint8_t *buffer) {
    // adc_read()
}

void gbcam_snap(uint8_t* buffer) {
    gbcam_reset();
    gbcam_initregs();
    gbcam_expose();

    gbcam_readsignal(buffer);
}
