/*
 * WS.h
 *
 *  Created on: Sep 12, 2024
 *      Author: indie
 */

#ifndef MYLIBRARIES_WS_H_
#define MYLIBRARIES_WS_H_

#include <bme680/bme68x_necessary_functions.h>

void WSStartScreen();
void WSShowTitle();
void WSShowTemp(float temp);
void WSShowPress(float press);
void WSShowHum(float hum);
void WSShowQual(float qual);
void WSShowAll(struct bme68x_data *data);

#endif /* MYLIBRARIES_WS_H_ */
