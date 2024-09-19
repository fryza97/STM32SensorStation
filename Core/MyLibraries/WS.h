/*
 * WS.h
 *
 *  Created on: Sep 12, 2024
 *      Author: indie
 */

#ifndef MYLIBRARIES_WS_H_
#define MYLIBRARIES_WS_H_

#include <bme680/bme68x_necessary_functions.h>
#include <../MyLibraries/RTC.h>

void WSStartScreen();
void WSShowTitle(struct RTC_values *rtc);
void WSShowTemp(float temp);
void WSShowPress(float press);
void WSShowHum(float hum);
void WSShowQual(float qual);
void WSShowAll(struct RTC_values *rtc, struct bme68x_data *data);
void WSMain(uint8_t *menu_state, uint8_t *before_hour, uint8_t *before_minutes, struct RTC_values *rtc, struct bme68x_data *data);

#endif /* MYLIBRARIES_WS_H_ */
