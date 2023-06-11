#include "gbcam.h"

#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"
#include "hardware/adc.h"

#include <string.h>


// 16KB, kind of brutal but we have way more than that
static uint8_t cameraData[GBCAM_H * GBCAM_W];

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

uint8_t* gbcam_snap() {
    // adc_read()
    return cameraData;
}
