/*
 * I2C.h
 *
 *  Created on: Aug 15, 2024
 *      Author: indie
 */

#ifndef MYLIBRARIES_I2C_H_
#define MYLIBRARIES_I2C_H_

#define DMA_TX DMA1
#define DMA_CHANNEL_TX LL_DMA_CHANNEL_6

#define DMA_RX DMA1
#define DMA_CHANNEL_RX LL_DMA_CHANNEL_7

void I2CInit();

//polling
void I2CSend(uint16_t slave_addr, uint16_t reg_addr,  uint8_t* data_ptr, uint16_t size);
void I2CRead(uint16_t slave_addr, uint16_t reg_addr,  uint8_t* data_ptr, uint16_t size);

//DMA
void I2CSendDMA(uint16_t slave_addr, uint16_t reg_addr, uint8_t* data_ptr, uint16_t size);
void I2CReadDMA(uint16_t slave_addr, uint16_t reg_addr, uint8_t* data_ptr, uint16_t size);
void I2CMemWriteDMA(uint16_t slave_addr, uint16_t reg_addr, uint8_t* data_ptr, uint16_t size);
void DMA_transmit_complete_callback(DMA_TypeDef* DMA_X, uint32_t DMA_CHANNEL_X);

#endif /* MYLIBRARIES_I2C_H_ */
