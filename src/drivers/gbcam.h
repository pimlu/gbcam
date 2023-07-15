#pragma once

#include <stdio.h>
#include "../constants.h"
#include "../pinout.h"

#define GBCAM_ROW_W (GBCAM_W/8)

void gbcam_init();
void gbcam_snap(uint8_t *buffer, uint16_t exposure);
