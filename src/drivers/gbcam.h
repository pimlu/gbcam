#pragma once

#include <stdio.h>

#define GBCAM_START 2
#define GBCAM_SIN 3
#define GBCAM_LOAD 7
#define GBCAM_XCK 8
#define GBCAM_RESET 9
#define GBCAM_READ 10
#define GBCAM_VOUT 26

#define GBCAM_VOUT_ADC 0

#define GBCAM_H 128
#define GBCAM_W 128
#define GBCAM_ROW_W (GBCAM_W/8)

void gbcam_init();
void gbcam_snap(uint8_t *buffer);
