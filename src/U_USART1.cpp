/*
 * Serial.cpp
 *
 *  Created on: 2016��1��18��
 *      Author: Romeli
 */
#include <stdlib.h>
#include <stdio.h>
#include <U_USART1.h>

uint8_t U_USART1::Rx_Buf[256];			//���ջ�����
uint8_t U_USART1::Rx_SP = 0;				//���ջ�����ָ��
uint8_t U_USART1::Read_SP = 0;			//��������ȡָ��
uint8_t U_USART1::Read_Available = 0;	//������δ���ֽ�

void U_USART1::Init(uint32_t baud) {
	GPIOInit();
	USARTInit(baud);
	NVICInit();

	USART_Cmd(USART1, ENABLE);
}
void U_USART1::print(int32_t data, uint8_t base) {
	uint8_t str[20];
	uint8_t len = byNumber(data, base, str);
	print(str, len);
}
//void U_USART1::print(double data, uint8_t ndigit) {
//	uint8_t str[20];
//	uint8_t len = byFloat(data, ndigit, str);
//	print(str, len);
//}
//
//void U_USART1::println(uint8_t *data) {
//	print(data);
//	print_c('\r');
//	print_c('\n');
//}
//void U_USART1::println(int32_t data) {
//	print(data);
//	print_c('\r');
//	print_c('\n');
//}
//void U_USART1::println(int16_t data) {
//	print(data);
//	print_c('\r');
//	print_c('\n');
//}
//void U_USART1::println(float data, uint8_t ndigit) {
//	print(data, ndigit);
//	print_c('\r');
//	print_c('\n');
//}
//void U_USART1::println(double data, uint8_t ndigit) {
//	print(data, ndigit);
//	print_c('\r');
//	print_c('\n');
//}

uint8_t U_USART1::print_c(uint8_t c) {
	USART_SendData(USART1, c);
	while (((USART1->ISR & USART_FLAG_TXE) == (uint16_t) RESET))
		;
	return 1;
}

void U_USART1::print(uint8_t* data) {
	while (*data != '\0') {
		print_c(*data);
		++data;
	}
}

void U_USART1::print(uint8_t* data, uint16_t len) {
	while (len--) {
		print_c(*data);
		++data;
	}
}

uint16_t U_USART1::available() {
	return USART1_Read_Available;
}

uint8_t U_USART1::read() {
	USART1_Read_Available--;
	return Rx_Buf[USART1_Read_SP++];
}

void U_USART1::read(uint8_t* buf, uint16_t len) {
	while (len--) {
		*buf = read();
		buf++;
	}
}

void U_USART1::GPIOInit() {
	GPIO_InitTypeDef GPIO_InitStructure;

	//Set PB6--USART_TX PB7--USART_RX
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void U_USART1::USARTInit(uint32_t baud) {
	USART_InitTypeDef USART_InitStructure;
	//配置 USART时钟和分频
	RCC_USARTCLKConfig(RCC_USART1CLK_PCLK);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	//全双工 无校验 停止位1位 8字节
	USART_InitStructure.USART_BaudRate = baud;
	USART_InitStructure.USART_HardwareFlowControl =
	USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;

	USART_Init(USART1, &USART_InitStructure);
}

void U_USART1::NVICInit() {
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;

	NVIC_Init(&NVIC_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}

extern "C" void USART1_IRQHandler(void) {
	if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)
		USART_ReceiveData(USART1);
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
		U_USART1::Rx_Buf[U_USART1::Rx_SP] = USART1->RDR & (uint16_t) 0x01FF;
		U_USART1::Rx_SP++;
		U_USART1::Read_Available++;
	}
}

