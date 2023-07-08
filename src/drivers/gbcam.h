#pragma once

#include <stdio.h>
#include "../pinout.h"

#define GBCAM_H 128
#define GBCAM_W 128
#define GBCAM_ROW_W (GBCAM_W/8)

void gbcam_init();
void gbcam_snap(uint8_t *buffer);
