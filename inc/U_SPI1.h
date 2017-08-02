/*
 * U_SPI1.h
 *
 *  Created on: 2017年7月20日
 *      Author: Romeli
 */

#ifndef U_SPI1_H_
#define U_SPI1_H_

#include "cmsis_device.h"

#define SPI1_Speed_18M SPI_BaudRatePrescaler_4 //only on SPI1
#define SPI1_Speed_9M SPI_BaudRatePrescaler_8
#define SPI1_Speed_4_5M SPI_BaudRatePrescaler_16
#define SPI1_Speed_2_25M SPI_BaudRatePrescaler_32
#define SPI1_Speed_1_125M SPI_BaudRatePrescaler_64
#define SPI1_Speed_562_5K SPI_BaudRatePrescaler_128
#define SPI1_Speed_281_25K SPI_BaudRatePrescaler_256

#define SPI1_TXBUF_SIZE 128
#define SPI1_RXBUF_SIZE 128

#define USE_DMA

class U_SPI1 {
public:
	static uint8_t TxBuf[SPI1_TXBUF_SIZE];
	static uint8_t TxBuf_SP;
	static uint8_t RxBuf[SPI1_RXBUF_SIZE];
	static uint8_t RxBuf_SP;
	static uint8_t DMARxBuf[SPI1_TXBUF_SIZE];
	static uint8_t DMARxBuf_SP;

	static volatile bool Busy;

	static void Init(uint16_t SPI1_Speed);
	static void SendAsync(uint8_t* data, uint16_t size);
private:
	static void GPIOInit();
	static void SPIInit(uint16_t SPI1_Speed);
	static void DMAInit();
	static void NVICInit();
	static void EXTIInit();
};

extern void SPI1_EVENT();

#endif /* U_SPI1_H_ */
