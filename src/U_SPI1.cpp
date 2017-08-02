/*
 * U_SPI1.cpp
 *
 *  Created on: 2017年7月20日
 *      Author: Romeli
 */

#include "U_SPI1.h"

uint8_t U_SPI1::TxBuf[SPI1_TXBUF_SIZE];
uint8_t U_SPI1::TxBuf_SP = 0;
uint8_t U_SPI1::RxBuf[SPI1_RXBUF_SIZE];
uint8_t U_SPI1::RxBuf_SP = 0;
uint8_t U_SPI1::DMARxBuf[SPI1_TXBUF_SIZE];
uint8_t U_SPI1::DMARxBuf_SP = 0;

volatile bool U_SPI1::Busy = false;

void U_SPI1::Init(uint16_t SPI1_Speed) {
	GPIOInit();
	SPIInit(SPI1_Speed);
	DMAInit();
	EXTIInit();
	NVICInit();
	SPI_Cmd(SPI1, ENABLE);
}

void U_SPI1::SendAsync(uint8_t* data, uint16_t size) {
#ifdef USE_DMA
	while (Busy)
		;
	Busy = true;
	for (uint16_t i = 0; i < size; ++i) {
		TxBuf[i] = data[i];
	}
	DMA1_Channel3->CMAR = (uint32_t) TxBuf;
	DMA1_Channel3->CNDTR = size;
	DMA_Cmd(DMA1_Channel3, ENABLE);
#else

#endif
}

void U_SPI1::GPIOInit() {
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_0);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_0);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_0);
}

void U_SPI1::SPIInit(uint16_t SPI1_Speed) {
	SPI_InitTypeDef SPI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI1_Speed;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;
	SPI_Init(SPI1, &SPI_InitStructure);

//	SPI_NSSInternalSoftwareConfig(SPI1, SPI_NSSInternalSoft_Reset);
	SPI_RxFIFOThresholdConfig(SPI1, SPI_RxFIFOThreshold_QF);
}

void U_SPI1::DMAInit() {
	DMA_InitTypeDef DMA_InitStructure;

	//开启DMA时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

	DMA_DeInit(DMA1_Channel3);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) (&SPI1->DR);
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) TxBuf; //临时设置，无效
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = 10; //临时设置，无效
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

	DMA_Init(DMA1_Channel3, &DMA_InitStructure);

	DMA_DeInit(DMA1_Channel2);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t) (&SPI1->DR);
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) DMARxBuf;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = SPI1_RXBUF_SIZE;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

	DMA_Init(DMA1_Channel2, &DMA_InitStructure);
	DMA_Cmd(DMA1_Channel2, ENABLE);

	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
	SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Rx, ENABLE);
}

void U_SPI1::NVICInit() {
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 1;
#ifdef USE_DMA
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel2_3_IRQn;
	NVIC_Init(&NVIC_InitStructure);

	DMA_ITConfig(DMA1_Channel3, DMA_IT_TC, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn;
	NVIC_Init(&NVIC_InitStructure);
#else
	NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
	NVIC_Init(&NVIC_InitStructure);

	SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_RXNE, ENABLE);
#endif
}

void U_SPI1::EXTIInit() {
	EXTI_InitTypeDef EXTI_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource15);

	EXTI_InitStructure.EXTI_Line = EXTI_Line15;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitStructure);
}

__attribute__((weak)) void SPI1_EVENT() {

}

extern "C" {

void SPI1_IRQHandler(void) {
	if (SPI_I2S_GetITStatus(SPI1, SPI_I2S_IT_RXNE)) {
		uint8_t tmp = (uint8_t) SPI1->DR;
		SPI_SendData8(SPI1, tmp + 1);
	}
}

void DMA1_Channel2_3_IRQHandler(void) {
	if (DMA_GetITStatus(DMA1_IT_TC3) != RESET) {
		DMA_ClearITPendingBit(DMA1_IT_TC3);
		DMA_Cmd(DMA1_Channel3, DISABLE);
		U_SPI1::Busy = false;
	}
	if (DMA_GetITStatus(DMA1_IT_TC2) != RESET) {
		DMA_ClearITPendingBit(DMA1_IT_TC2);
		DMA_Cmd(DMA1_Channel2, ENABLE);
	}
}

void EXTI4_15_IRQHandler(void) {
	if (EXTI_GetITStatus(EXTI_Line15) != RESET) {
		DMA_Cmd(DMA1_Channel2, DISABLE);
		U_SPI1::RxBuf_SP = SPI1_RXBUF_SIZE - DMA1_Channel2->CNDTR;
		for (uint16_t i = 0; i < U_SPI1::RxBuf_SP; i++) {
			U_SPI1::RxBuf[i] = U_SPI1::DMARxBuf[i];
		}
		DMA1_Channel2->CNDTR = SPI1_RXBUF_SIZE;
		DMA_Cmd(DMA1_Channel2, ENABLE);
		SPI1_EVENT();
		EXTI_ClearITPendingBit(EXTI_Line15);
	}
}

}
