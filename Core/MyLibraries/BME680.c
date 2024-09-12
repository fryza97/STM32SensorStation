/*
 * BME680.c
 *
 *  Created on: Sep 1, 2024
 *      Author: indie
 */


#include "main.h"
#include <bme680/bme68x_necessary_functions.h>
#include "BME680.h"

void BME680init(struct bme68x_data *data){
	bme68x_start(data);
}

void BME680measurment(struct bme68x_data *data){
	if (bme68x_single_measure(data) == 0) {

		// Measurement is successful, so continue with IAQ
		data->iaq_score = bme68x_iaq(); // Calculate IAQ
		data->pressure = (data->pressure)*0.01f;

		LL_mDelay(2000);
	}
}
