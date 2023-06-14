#pragma once

#include <stdio.h>

#define GBCAM_START 29
#define GBCAM_SIN 27
#define GBCAM_LOAD 26
#define GBCAM_XRST 25
#define GBCAM_XCK 24
#define GBCAM_RESET 22
#define GBCAM_READ 31
#define GBCAM_VOUT 28

#define GBCAM_VOUT_ADC 2

#define GBCAM_H 128
#define GBCAM_W 128
#define GBCAM_ROW_W (GBCAM_W/8)

void gbcam_init();
void gbcam_snap(uint8_t *buffer);
