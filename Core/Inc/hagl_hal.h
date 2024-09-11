/*
 * hagl_hal.h
 *
 *  Created on: Sep 10, 2024
 *      Author: indie
 */

#pragma once

#include "main.h"
#include "../MyLibraries/LCD.h"

#define DISPLAY_WIDTH 	(LCD_WIDTH)
#define DISPLAY_HEIGHT 	(LCD_HEIGHT)
#define DISPLAY_DEPTH 	16

typedef uint16_t color_t;

#define hagl_hal_put_pixel LCDPutPixel
