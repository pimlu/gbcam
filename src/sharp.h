#pragma once

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/spi.h"

#define spi_sharp spi0


#define SHARP_RX 20
#define SHARP_SCK 6
#define SHARP_TX 19
#define SHARP_CSN 1

#define SHARP_H 168
#define SHARP_W 144

// https://www.sharpsde.com/fileadmin/products/Displays/2016_SDE_App_Note_for_Memory_LCD_programming_V1.3.pdf
#define DATA_ROW_W (SHARP_W/8)
#define LINE_W (1 + DATA_ROW_W + 1)

#define CMD_ROW_W (1 + LINE_W + 1)
#define CMD_W (1 + SHARP_H * LINE_W + 1)

void spi_sharp_init();

void draw(bool vcom);
