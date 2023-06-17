#pragma once

#include <stdio.h>


#define GBCAM_START 2 // green
#define GBCAM_SIN 3 // yellow
#define GBCAM_LOAD 7 // white
#define GBCAM_XCK 8 // yellow
#define GBCAM_RESET 9 // green
#define GBCAM_READ 10 // blue
#define GBCAM_SHIFT_EN 11 // white
#define GBCAM_VOUT 26 // blue


#define GBCAM_VOUT_ADC 0

#define GBCAM_H 128
#define GBCAM_W 128
#define GBCAM_ROW_W (GBCAM_W/8)

void gbcam_init();
void gbcam_snap(uint8_t *buffer);
