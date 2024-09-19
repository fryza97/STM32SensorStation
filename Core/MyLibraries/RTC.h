/*
 * RTC.h
 *
 *  Created on: Sep 12, 2024
 *      Author: indie
 */

#ifndef MYLIBRARIES_RTC_H_
#define MYLIBRARIES_RTC_H_

struct RTC_values{
	uint8_t year;
	uint8_t day;
	uint8_t month;
	uint8_t date;

	uint8_t hour;
	uint8_t minutes;
	uint8_t seconds;
};

void RTCRead(struct RTC_values* rtc);
void RTCSet(struct RTC_values* rtc);

#endif /* MYLIBRARIES_RTC_H_ */
