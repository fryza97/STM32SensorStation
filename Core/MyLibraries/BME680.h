/*
 * BME680.h
 *
 *  Created on: Sep 1, 2024
 *      Author: indie
 */

#ifndef MYLIBRARIES_BME680_H_
#define MYLIBRARIES_BME680_H_

#include <bme680/bme68x_necessary_functions.h>

void BME680init(struct bme68x_data *data);
void BME680measurment(struct bme68x_data *data);

#endif /* MYLIBRARIES_BME680_H_ */
