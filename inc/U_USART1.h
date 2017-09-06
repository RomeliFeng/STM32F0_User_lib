/*
 * Serial.h
 *
 *  Created on: 2016��1��18��
 *      Author: Romeli
 *      Version: V1.0
 */

#ifndef SERIAL_H_
#define SERIAL_H_

#include "stm32f0xx.h"
#include "Parse.h"

extern char USART1_RX_Buf[256];			//���ջ�����
extern uint8_t USART1_RX_SP;			//���ջ�����ָ��
extern uint8_t USART1_Read_SP;			//��������ȡָ��
extern uint8_t USART1_Read_Available;	//������δ���ֽ�

class U_USART1: Parse {
public:
	static uint8_t Rx_Buf[256];
	static uint8_t Rx_SP;
	static uint8_t Read_SP;
	static uint8_t Read_Available;

	static void Init(uint32_t baud);

	static uint8_t print_c(uint8_t c);
	static void print(uint8_t* data);

	static inline void print(const char* data) {
		print((uint8_t*) data);
	}
	static void print(uint8_t* data, uint16_t len);

	static void print(int32_t data, uint8_t base = 10);

	static uint16_t available();
	static uint8_t read();
	static void read(uint8_t *buf, uint16_t len);
private:
	static void GPIOInit();
	static void USARTInit(uint32_t baud);
	static void NVICInit();
};
#endif /* SERIAL_H_ */
