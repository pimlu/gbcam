#include "gbcam.h"

#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"

#include <string.h>

void gbcam_init() {
    gpio_init(GBCAM_START);
    gpio_set_dir(GBCAM_START, GPIO_OUT);
    gpio_init(GBCAM_SIN);
    gpio_set_dir(GBCAM_SIN, GPIO_OUT);
    gpio_init(GBCAM_LOAD);
    gpio_set_dir(GBCAM_LOAD, GPIO_OUT);
    gpio_init(GBCAM_XCK);
    gpio_set_dir(GBCAM_XCK, GPIO_OUT);
    gpio_put(GBCAM_RESET, 0);
    gpio_init(GBCAM_RESET);
    gpio_set_dir(GBCAM_RESET, GPIO_OUT);

    // the SN74LVC245A is active low, make sure it starts high
    gpio_put(GBCAM_SHIFT_EN, 1);
    gpio_init(GBCAM_SHIFT_EN);
    gpio_set_dir(GBCAM_SHIFT_EN, GPIO_OUT);

    gpio_init(GBCAM_READ);
    gpio_set_dir(GBCAM_READ, GPIO_IN);

    adc_gpio_init(GBCAM_VOUT);
    adc_select_input(GBCAM_VOUT_ADC);
}

// from the datasheet, example image capture mode
// "Positive image (without dark pixel output)"
static uint8_t registers[8] = { 0x80, 0x04, 0x20, 0x00, 0xa1, 0x10, 0x01, 0x03 };

static inline void gbclk(bool b) {
    busy_wait_us(1);
    gpio_put(GBCAM_XCK, b);
    busy_wait_us(1);
}

void __not_in_flash_func(send_bits)(uint n, uint bits) {
    uint mask = 1 << (bits - 1);
    for (uint b = 0; b < bits; b++) {
        gpio_put(GBCAM_SIN, !!(n & mask));
        mask >>= 1;
        gbclk(1);
        gbclk(0);
    }
}

static void __not_in_flash_func(gbcam_reset)() {
    gpio_put(GBCAM_SHIFT_EN, 0);

    gbclk(0);
    gpio_put(GBCAM_RESET, 0);

    // diagram timing has one clock while resetting
    gbclk(1);
    gbclk(0);

    gpio_put(GBCAM_RESET, 1);
}

static void __not_in_flash_func(gbcam_initregs)() {
    // static uint8_t x = 0;
    // registers[2] = x;
    // registers[3] = x;
    // printf("#### r[2]: %d", x++);
    for (uint r = 0; r < 8; r++) {
        send_bits(r, 3);
        send_bits(registers[r], 7);

        gpio_put(GBCAM_LOAD, 1);
        gbclk(1);
        // last bit has to be done separately since the LOAD pin is high
        gpio_put(GBCAM_SIN, registers[r] >> 7);

        gbclk(0);
        gpio_put(GBCAM_LOAD, 0);
    }
}

static void __not_in_flash_func(gbcam_expose)() {
    gpio_put(GBCAM_START, 1);
    gbclk(1);
    gpio_put(GBCAM_START, 0);
    gbclk(0);

    while (gpio_get(GBCAM_READ) == 0) {
        gbclk(0);
        gbclk(1);
    }
}

static inline void trigger_adc_read() {
    hw_set_bits(&adc_hw->cs, ADC_CS_START_ONCE_BITS);
}
static inline uint16_t await_adc_result() {
    while (!(adc_hw->cs & ADC_CS_READY_BITS))
        tight_loop_contents();

    return (uint16_t) adc_hw->result;
}

static void __not_in_flash_func(gbcam_readsignal)(uint8_t *buffer) {
    uint i = 0;
    while (i < (GBCAM_H * GBCAM_W) && gpio_get(GBCAM_READ) == 1) {
        gbclk(0);
        // make the adc start reading, which takes 96 cycles (0.7us)
        trigger_adc_read();
        // gbclk will sleep for 2us
        gbclk(1);
        // therefore await_adc_result shouldn't wait at all, if things go nominally
        uint16_t val = await_adc_result();
        buffer[i++] = (uint8_t) (val >> 4);
    }
}

void __not_in_flash_func(gbcam_snap)(uint8_t* buffer) {
    gbcam_reset();
    gbcam_initregs();
    gbcam_expose();

    gbcam_readsignal(buffer);
}
