/*
 * BME680.c
 *
 *  Created on: Sep 1, 2024
 *      Author: indie
 */


#include "main.h"
#include <bme680/bme68x_necessary_functions.h>
#include "BME680.h"

struct bme68x_data data;

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

void BME680show(){
	 printf("\r\n");
	 printf("\r\n");
	 printf("Mesurement: \r\n");
	 printf("-temperature: %2.2f st. C\r\n", data.temperature);
	 printf("-pressure: %3.2f hPa\r\n", data.pressure);
	 printf("-humidity: %2.2f %% \r\n", data.humidity);
	 printf("-air quality: %2.0f IAQ\r\n", data.iaq_score);
}
