/*
 * I2C.c
 *
 *  Created on: Aug 15, 2024
 *      Author: indie
 */

#include "main.h"
#include "I2C.h"

I2C_TypeDef *i2c = I2C1;

void I2CInit(){
	LL_I2C_Enable(i2c);
	LL_DMA_EnableIT_TC(DMA_TX, DMA_CHANNEL_TX);
	LL_DMA_EnableIT_TC(DMA_RX, DMA_CHANNEL_RX);
}

//polling
void I2CSend(uint16_t slave_addr, uint8_t* data_ptr, uint16_t size){

	uint8_t data_count = size;

	LL_I2C_HandleTransfer(i2c, slave_addr, LL_I2C_ADDRSLAVE_7BIT, (uint32_t)size, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_WRITE);

	while(data_count > 0){
		while(LL_I2C_IsActiveFlag_TXIS(i2c) == 0);

		LL_I2C_TransmitData8(i2c, *data_ptr);

		data_ptr++;
		data_count--;
	}

	while(LL_I2C_IsActiveFlag_STOP(i2c) == 0);
	LL_I2C_ClearFlag_STOP(i2c);
}

void I2CRead(uint16_t slave_addr,  uint8_t* data_ptr, uint16_t size){

	uint8_t data_count = size;

	LL_I2C_HandleTransfer(i2c, slave_addr, LL_I2C_ADDRSLAVE_7BIT, (uint32_t)size, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_READ);

	while(data_count > 0){
		while(LL_I2C_IsActiveFlag_RXNE(i2c) == 0);

		*data_ptr = LL_I2C_ReceiveData8(i2c);

		data_ptr++;
		data_count--;
	}

	while(LL_I2C_IsActiveFlag_STOP(i2c) == 0);

	LL_I2C_ClearFlag_STOP(i2c);
}

//DMA
void I2CSendDMA(uint16_t slave_addr, uint16_t reg_addr, uint8_t* data_ptr, uint16_t size){

	LL_I2C_HandleTransfer(i2c, slave_addr, LL_I2C_ADDRSLAVE_7BIT, (uint32_t)(size+1), LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_WRITE);

	while(LL_I2C_IsActiveFlag_TXIS(i2c) == 0);

	LL_I2C_TransmitData8(i2c, (uint8_t)reg_addr);

	LL_DMA_ConfigAddresses(DMA_TX, DMA_CHANNEL_TX, (uint32_t)data_ptr, LL_I2C_DMA_GetRegAddr(i2c, LL_I2C_DMA_REG_DATA_TRANSMIT), LL_DMA_GetDataTransferDirection(DMA_TX, DMA_CHANNEL_TX));
	LL_DMA_SetDataLength(DMA_TX, DMA_CHANNEL_TX, size);

	LL_I2C_EnableDMAReq_TX(i2c);
	LL_DMA_EnableChannel(DMA_TX, DMA_CHANNEL_TX);
}


void I2CReadDMA(uint16_t slave_addr, uint16_t reg_addr, uint8_t* data_ptr, uint16_t size){

	LL_I2C_HandleTransfer(i2c, slave_addr, LL_I2C_ADDRSLAVE_7BIT, 1, LL_I2C_MODE_SOFTEND, LL_I2C_GENERATE_START_WRITE);

	while(LL_I2C_IsActiveFlag_TXIS(i2c) == 0);

	LL_I2C_TransmitData8(i2c, (uint8_t)reg_addr);

	while(LL_I2C_IsActiveFlag_TC(i2c) == 0);

	LL_I2C_HandleTransfer(i2c, slave_addr, LL_I2C_ADDRSLAVE_7BIT, (uint32_t)size, LL_I2C_MODE_AUTOEND, LL_I2C_GENERATE_START_READ);

	LL_DMA_ConfigAddresses(DMA_RX, DMA_CHANNEL_RX, LL_I2C_DMA_GetRegAddr(i2c, LL_I2C_DMA_REG_DATA_RECEIVE), (uint32_t)data_ptr, LL_DMA_GetDataTransferDirection(DMA_RX, DMA_CHANNEL_RX));
	LL_DMA_SetDataLength(DMA_RX, DMA_CHANNEL_RX, size);

	LL_I2C_EnableDMAReq_RX(i2c);
	LL_DMA_EnableChannel(DMA_RX, DMA_CHANNEL_RX);
}

void DMA_transmit_complete_callback(DMA_TypeDef* DMA_X, uint32_t DMA_CHANNEL_X)
{
	LL_DMA_DisableChannel(DMA_X, DMA_CHANNEL_X);
	LL_I2C_ClearFlag_STOP(i2c);
}
