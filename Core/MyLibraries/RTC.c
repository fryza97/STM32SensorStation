/*
 * RTC.c
 *
 *  Created on: Sep 12, 2024
 *      Author: indie
 */

#include "main.h"
#include "RTC.h"

static uint8_t byte2bcd(uint8_t byte)
{
  uint8_t bcdhigh = 0;
  while (byte >= 10)
  {
    bcdhigh++;
    byte -= 10;
  }
  return  ((uint8_t)(bcdhigh << 4) | byte);
}

static uint8_t bcd2byte(uint8_t bcd)
{
  uint8_t tmp = 0;
  tmp = ((uint8_t)(bcd & (uint8_t)0xF0) >> (uint8_t)0x4) * 10;
  return (tmp + (bcd & (uint8_t)0x0F));
}


void RTCRead(struct RTC_values* rtc){
	rtc->hour   = bcd2byte((RTC->TR & (RTC_TR_HT_Msk  | RTC_TR_HU_Msk )) >>RTC_TR_HU_Pos);
	rtc->minutes = bcd2byte((RTC->TR & (RTC_TR_MNT_Msk | RTC_TR_MNU_Msk)) >>RTC_TR_MNU_Pos);
	rtc->seconds = bcd2byte((RTC->TR & (RTC_TR_ST_Msk  | RTC_TR_SU_Msk )) >>RTC_TR_SU_Pos);

	rtc->year	= bcd2byte((RTC->DR & (RTC_DR_YT_Msk  | RTC_DR_YU_Msk )) >>RTC_DR_YU_Pos);
	rtc->day	= (RTC->DR & RTC_DR_WDU_Msk) >> RTC_DR_WDU_Pos;
	rtc->month	= bcd2byte((RTC->DR & (RTC_DR_MT_Msk | RTC_DR_MU_Msk)) >>RTC_DR_MU_Pos);
	rtc->date	= bcd2byte((RTC->DR & (RTC_DR_DT_Msk  | RTC_DR_DU_Msk )) >>RTC_DR_DU_Pos);
}

void RTCSet(struct RTC_values* rtc){
	uint32_t bcdtime;
	uint32_t bcddate;

	bcdtime = ( (byte2bcd(rtc->hour) << 16U) |
				(byte2bcd(rtc->minutes) << 8U) |
				(byte2bcd(rtc->seconds) << 0U) );

	bcddate = ( (byte2bcd(rtc->year) << 16U) |
				(rtc->day & 0b111) << 13U |
				(byte2bcd(rtc->month) << 8U) |
				(byte2bcd(rtc->date) << 0U) );

	// Enable Write access for RTC registers
	RTC->WPR = 0xCA;
	RTC->WPR = 0x53;

	RTC->ISR |= RTC_ISR_INIT;
	while((RTC->ISR & RTC_ISR_INITF) != RTC_ISR_INITF);

	// Setup prescalers for 1s RTC clock
	RTC->PRER = 0x007F00FF;

	// Set time
	RTC->TR = bcdtime;

	//Set date
	RTC->DR = bcddate;

	// Exit Init
	RTC->ISR &= ~RTC_ISR_INIT;

	// Disable Write access for RTC registers
	RTC->WPR = 0xFE;
	RTC->WPR = 0x64;
}
