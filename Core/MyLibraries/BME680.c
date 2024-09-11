/*
 * BME680.c
 *
 *  Created on: Sep 1, 2024
 *      Author: indie
 */


#include "main.h"
#include <bme680/bme68x_necessary_functions.h>
#include "BME680.h"
#include "../MyLibraries/LCD.h"
#include "hagl.h"
#include "font9x15.h"
#include "font6x9.h"
#include "rgb565.h"
#include <stdio.h>
#include <wchar.h>

#define DARKGRAY 0x2842
#define BACKGROUND 0xDFEF
#define TITLE 0x1ff8
#define TEMPERATURE 0xAAFB
#define PRESSURE 0x7F46
#define HUMIDITY 0x4c8f
#define QUALITY 0x3FFE

struct bme68x_data data;
char buffer[100];

wchar_t* formatString(wchar_t* message, float value) {
    static wchar_t buffer[100];
    swprintf(buffer, sizeof(buffer), message, value);
    return buffer;
}

void refresh_title(){
	hagl_fill_rectangle(0, 0, 160, 25, BLACK);
	hagl_draw_rectangle(0, 0, 100, 26, TITLE);
	hagl_draw_rectangle(101, 0, 159, 26, RED);
}

void refresh_temp(){
	hagl_fill_rounded_rectangle(5, 35, 75, 75, 5, BLACK);
	hagl_draw_rounded_rectangle(5, 35, 76, 75, 5, TEMPERATURE);
}

void refresh_press(){
	hagl_fill_rounded_rectangle(85, 35, 155, 75, 5, BLACK);
	hagl_draw_rounded_rectangle(85, 35, 156, 75, 5, PRESSURE);
}

void refresh_hum(){
	hagl_fill_rounded_rectangle(5, 82, 75, 122, 5, BLACK);
	hagl_draw_rounded_rectangle(5, 82, 76, 122, 5, HUMIDITY);
}

void refresh_qual(){
	hagl_fill_rounded_rectangle(85,	82, 155, 122, 5, BLACK);
	hagl_draw_rounded_rectangle(85,	82, 156, 122, 5, QUALITY);
}

void BME680init(){
	bme68x_start(&data);
}

void BME680measurment(){
	if (bme68x_single_measure(&data) == 0) {

		// Measurement is successful, so continue with IAQ
		data.iaq_score = bme68x_iaq(); // Calculate IAQ
		data.pressure = data.pressure*0.01f;

		LL_mDelay(2000);
	}
}

void BME680start(){
	hagl_fill_rectangle(0, 0, 160, 128, DARKGRAY);
	refresh_title();
	refresh_temp();
	refresh_hum();
	refresh_press();
	refresh_qual();

	hagl_put_text(L"Welcome!", 15, 6, TITLE, BLACK, font6x9);

	LCDCopy();
}

void BME680show(){
	refresh_title();
	hagl_put_text(L"10-09-2024", 5, 6, TITLE, BLACK, font9x15);
	hagl_put_text(L"21:32", 108, 6, RED, BLACK, font9x15);

	refresh_temp();
	hagl_put_text(formatString(L"%2.2f", data.temperature), 18, 40, TEMPERATURE, BLACK, font9x15);
	hagl_put_text(L"st. C", 18, 55, TEMPERATURE, BLACK, font9x15);

	refresh_press();
	hagl_put_text(formatString(L"%3.2f", data.pressure), 90, 40, PRESSURE, BLACK, font9x15);
	hagl_put_text(L"hPa", 110, 55, PRESSURE, BLACK, font9x15);

	refresh_hum();
	hagl_put_text(formatString(L"%2.2f %%", data.humidity), 10, 94, HUMIDITY, BLACK, font9x15);

	refresh_qual();
	hagl_put_text(formatString(L"%2.0f IAQ", data.iaq_score), 95, 94, QUALITY, BLACK, font9x15);

	LCDCopy();
}
