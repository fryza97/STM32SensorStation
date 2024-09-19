/*
 * WS.c
 *
 *  Created on: Sep 12, 2024
 *      Author: indie
 */

#include "main.h"
#include "WS.h"
#include <bme680/bme68x_necessary_functions.h>
#include "../MyLibraries/LCD.h"
#include "hagl.h"
#include "font9x15.h"
#include "rgb565.h"
#include <stdio.h>
#include <wchar.h>
#include <../MyLibraries/RTC.h>
#include <../MyLibraries/BME680.h>

#define DARKGRAY 0x2842
#define BACKGROUND 0xDFEF
#define TITLE 0x1ff8
#define TEMPERATURE 0xAAFB
#define PRESSURE 0x7F46
#define HUMIDITY 0x4c8f
#define QUALITY 0x3FFE

extern uint8_t interrupt0, interrupt1, interrupt2;

static wchar_t* formatString(wchar_t* message, float value) {
    static wchar_t buffer[100];
    swprintf(buffer, sizeof(buffer), message, value);
    return buffer;
}

static wchar_t* formatInt(wchar_t* message, int value) {
    static wchar_t buffer[100];
    swprintf(buffer, sizeof(buffer), message, value);
    return buffer;
}

static wchar_t* formatDate(struct RTC_values *rtc) {
    static wchar_t buffer[100];
    swprintf(buffer, sizeof(buffer) / sizeof(wchar_t), L"%02d-%02d-%04d",
             (int)rtc->date, (int)rtc->month, (int)rtc->year + 2000);
    return buffer;
}

static wchar_t* formatTime(struct RTC_values *rtc) {
    static wchar_t buffer[100];
    swprintf(buffer, sizeof(buffer) / sizeof(wchar_t), L"%02d:%02d", (int)rtc->hour, (int)rtc->minutes);
    return buffer;
}

static void refresh_title(){
	hagl_fill_rectangle(0, 0, 160, 25, BLACK);
	hagl_draw_rectangle(0, 0, 100, 26, TITLE);
	hagl_draw_rectangle(101, 0, 159, 26, RED);
}

static void refresh_temp(){
	hagl_fill_rounded_rectangle(5, 35, 75, 75, 5, BLACK);
	hagl_draw_rounded_rectangle(5, 35, 76, 75, 5, TEMPERATURE);
}

static void refresh_press(){
	hagl_fill_rounded_rectangle(85, 35, 155, 75, 5, BLACK);
	hagl_draw_rounded_rectangle(85, 35, 156, 75, 5, PRESSURE);
}

static void refresh_hum(){
	hagl_fill_rounded_rectangle(5, 82, 75, 122, 5, BLACK);
	hagl_draw_rounded_rectangle(5, 82, 76, 122, 5, HUMIDITY);
}

static void refresh_qual(){
	hagl_fill_rounded_rectangle(85,	82, 155, 122, 5, BLACK);
	hagl_draw_rounded_rectangle(85,	82, 156, 122, 5, QUALITY);
}

void WSStartScreen(){
	hagl_fill_rectangle(0, 0, 160, 128, DARKGRAY);
	refresh_title();
	refresh_temp();
	refresh_hum();
	refresh_press();
	refresh_qual();

	hagl_put_text(L"Welcome!", 5, 6, TITLE, BLACK, font9x15);

	LCDCopy();
}

void WSShowTitle(struct RTC_values *rtc){
	refresh_title();
	hagl_put_text(formatDate(rtc), 5, 6, TITLE, BLACK, font9x15);
	hagl_put_text(formatTime(rtc), 108, 6, RED, BLACK, font9x15);
}

void WSShowTemp(float temp){
	refresh_temp();
	hagl_put_text(formatString(L"%2.2f", temp), 18, 40, TEMPERATURE, BLACK, font9x15);
	hagl_put_text(L"st. C", 18, 55, TEMPERATURE, BLACK, font9x15);
}

void WSShowPress(float press){
	refresh_press();
	hagl_put_text(formatString(L"%3.2f", press), 90, 40, PRESSURE, BLACK, font9x15);
	hagl_put_text(L"hPa", 110, 55, PRESSURE, BLACK, font9x15);
}

void WSShowHum(float hum){
	refresh_hum();
	hagl_put_text(formatString(L"%2.2f %%", hum), 10, 94, HUMIDITY, BLACK, font9x15);
}

void WSShowQual(float qual){
	refresh_qual();
	if(qual < 51)
		hagl_put_text(L"BEST", 102, 95, WHITE, BLACK, font9x15);
	else if(qual < 101)
		hagl_put_text(L"GOOD", 102, 95, GREEN, BLACK, font9x15);
	else if(qual < 201)
		hagl_put_text(L"FAIR", 102, 95, YELLOW, BLACK, font9x15);
	else if(qual < 301)
		hagl_put_text(L"POOR", 102, 95, ORANGE, BLACK, font9x15);
	else
		hagl_put_text(L"BAD", 107, 95, RED, BLACK, font9x15);
}

void WSShowAll(struct RTC_values *rtc, struct bme68x_data *data){
	WSShowTitle(rtc);
	WSShowTemp(data -> temperature);
	WSShowPress(data -> pressure);
	WSShowHum(data -> humidity);
	WSShowQual(data -> iaq_score);
	LCDCopy();
}

void WSMain(uint8_t *menu_state, uint8_t *before_hour, uint8_t *before_minutes, struct RTC_values *rtc, struct bme68x_data *data){
	RTCRead(rtc);
	switch(*menu_state){
	case 0:
		if(rtc->hour - *before_hour > 0){
			*before_hour = rtc->hour;
			*before_minutes = rtc->minutes;
			BME680measurment(data);
			WSShowAll(rtc, data);
		}
		else if(rtc->minutes - *before_minutes > 0){
			*before_minutes = rtc->minutes;
			WSShowTitle(rtc);
			LCDCopy();
		}
	break;
	case 1:
		uint8_t sub_menu = 1;
		color_t dt, mnt, yr, hr, min;
		*menu_state = 0;
		uint8_t change = 1;
		struct RTC_values setter = *rtc;
		while(sub_menu > 0){
			if(sub_menu == 1){ //set year
				dt = TITLE;
				mnt = TITLE;
				yr = YELLOW;
				hr = RED;
				min = RED;

				if(interrupt0 == 1){
					interrupt0 = 0;
					setter.year--;
					change = 1;
				}
				if(interrupt1 == 1){
					interrupt1 = 0;
					sub_menu++;
					change = 1;
				}
				if(interrupt2 == 1){
					interrupt2 = 0;
					setter.year++;
					change = 1;
				}
			}
			if(sub_menu == 2){ //set month
				dt = TITLE;
				mnt = YELLOW;
				yr = TITLE;
				hr = RED;
				min = RED;

				if(interrupt0 == 1){
					interrupt0 = 0;
					setter.month--;
					change = 1;
				}
				if(interrupt1 == 1){
					interrupt1 = 0;
					sub_menu++;
					change = 1;
				}
				if(interrupt2 == 1){
					interrupt2 = 0;
					setter.month++;
					change = 1;
				}
			}
			if(sub_menu == 3){ //set date
				dt = YELLOW;
				mnt = TITLE;
				yr = TITLE;
				hr = RED;
				min = RED;

				if(interrupt0 == 1){
					interrupt0 = 0;
					setter.date--;
					change = 1;
				}
				if(interrupt1 == 1){
					interrupt1 = 0;
					sub_menu++;
					change = 1;
				}
				if(interrupt2 == 1){
					interrupt2 = 0;
					setter.date++;
					change = 1;
				}
			}
			if(sub_menu == 4){ //set hour
				dt = TITLE;
				mnt = TITLE;
				yr = TITLE;
				hr = YELLOW;
				min = RED;

				if(interrupt0 == 1){
					interrupt0 = 0;
					setter.hour--;
					change = 1;
				}
				if(interrupt1 == 1){
					interrupt1 = 0;
					sub_menu++;
					change = 1;
				}
				if(interrupt2 == 1){
					interrupt2 = 0;
					setter.hour++;
					change = 1;
				}
			}
			if(sub_menu == 5){ //set minutes
				dt = TITLE;
				mnt = TITLE;
				yr = TITLE;
				hr = RED;
				min = YELLOW;

				if(interrupt0 == 1){
					interrupt0 = 0;
					setter.minutes--;
					change = 1;
				}
				if(interrupt1 == 1){
					interrupt1 = 0;
					sub_menu = 0;
					change = 1;
				}
				if(interrupt2 == 1){
					interrupt2 = 0;
					setter.minutes++;
					change = 1;
				}
			}

			if(setter.month == 0) setter.month = 1;
			if(setter.month > 12) setter.month = 12;

			if(setter.date == 0) setter.date = 1;
			if(setter.date > 31) setter.date = 31;
			if((setter.month == 4 || setter.month == 6 || setter.month ==9 || setter.month == 11) && setter.date > 30) setter.date = 30;
			if(setter.month == 2 && ((setter.year+2000)%4 == 0) && setter.date > 29) setter.date = 29;
			if(setter.month == 2 && ((setter.year+2000)%4 != 0) && setter.date > 28) setter.date = 28;

			if(setter.hour > 200) setter.hour = 0;
			if(setter.hour > 23) setter.hour = 23;
			if(setter.minutes > 200) setter.minutes = 0;
			if(setter.minutes > 59) setter.minutes = 59;

			setter.day = 0;
			setter.seconds = 0;

			if(sub_menu == 0){
				*rtc = setter;
				*before_hour = rtc->hour;
				*before_minutes = rtc->minutes;
				RTCSet(rtc);
				WSShowAll(rtc, data);
			}
			else if(sub_menu != 0 && change == 1){
				change = 0;

				hagl_put_text(formatInt(L"%02d", (int)setter.date), 5, 6, dt, BLACK, font9x15);
				hagl_put_text(L"-", 23, 6, TITLE, BLACK, font9x15);
				hagl_put_text(formatInt(L"%02d", (int)setter.month), 32, 6, mnt, BLACK, font9x15);
				hagl_put_text(L"-", 50, 6, TITLE, BLACK, font9x15);
				hagl_put_text(formatInt(L"%04d", (int)((setter.year)+2000)), 59, 6, yr, BLACK, font9x15);

				hagl_put_text(formatInt(L"%02d",(int) setter.hour), 108, 6, hr, BLACK, font9x15);
				hagl_put_text(L":", 126, 6, RED, BLACK, font9x15);
				hagl_put_text(formatInt(L"%02d", (int)setter.minutes), 135, 6, min, BLACK, font9x15);

				LCDCopy();

				//printf("%02d-%02d-%04d  %02d:%02d\r\n", (int)setter.date, (int)setter.month, (int)setter.year + 2000, (int)setter.hour, (int)setter.minutes);
			}
		}
	break;
	}
}

















