/*
 * U_SPI1.h
 *
 *  Created on: 2017年7月20日
 *      Author: Romeli
 */

#ifndef U_SPI1_H_
#define U_SPI1_H_

#include "cmsis_device.h"

#define SPI1_Speed_24M SPI_BaudRatePrescaler_2
#define SPI1_Speed_12M SPI_BaudRatePrescaler_4
#define SPI1_Speed_6M SPI_BaudRatePrescaler_8
#define SPI1_Speed_3M SPI_BaudRatePrescaler_16
#define SPI1_Speed_1_5M SPI_BaudRatePrescaler_32
#define SPI1_Speed_750K SPI_BaudRatePrescaler_64
#define SPI1_Speed_375K SPI_BaudRatePrescaler_128
#define SPI1_Speed_182_25K SPI_BaudRatePrescaler_256

#define SPI1_TXBUF_SIZE 128
#define SPI1_RXBUF_SIZE 128

#define USE_DMA
#define USE_NSS

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
	static void SendSync(uint8_t* data, uint16_t size);
	static uint8_t SendSync(uint8_t data);
private:
	static void GPIOInit();
	static void SPIInit(uint16_t SPI1_Speed);
	static void DMAInit();
	static void NVICInit();
	static void EXTIInit();
};

extern void SPI1_EVENT();

#endif /* U_SPI1_H_ */
