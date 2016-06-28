/*
 * Serial.cpp
 *
 *  Created on: 2016年1月18日
 *      Author: Romeli
 */
#include "Serial.h"
#include <stdlib.h>
#include <stdio.h>

SerialClass Serial;

char USART1_RX_Buf[256];			//接收缓存器
uint8_t USART1_RX_SP = 0;				//接收缓存器指针
uint8_t USART1_Read_SP = 0;			//缓存器读取指针
uint8_t USART1_Read_Available = 0;	//缓冲器未读字节

void SerialClass::begin(uint32_t BaudRate)
{
	/*定义初始化用结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/*开启GPIOA和USART1的时钟*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_USARTCLKConfig(RCC_USART1CLK_PCLK);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	/*设置PA9为复用推挽输出*/
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;

	/*设置PA10为浮动输入*/
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);

	/*串口初始化，波特率：参数1；无自动控制；双向；停止位：1位；字节：8；*/
	USART_InitStructure.USART_BaudRate = BaudRate;
	USART_InitStructure.USART_HardwareFlowControl =
	USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;

	USART_Init(USART1, &USART_InitStructure);

	/*配置中断优先级为组1  1位抢占 3位响应*/
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0;

	NVIC_Init(&NVIC_InitStructure);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	USART_Cmd(USART1, ENABLE);
	USART_ClearFlag(USART1, USART_FLAG_TXE);
}
//void SerialClass::print(char *format,...)
//{
//	va_list argp;
//	va_start(argp,format);
//	printf(format,argp);
//	va_end(argp);
//}

void SerialClass::print(char *data)
{
	print_s(data);
}
void SerialClass::print(long data)
{
	char str[20];
	sprintf(str, "%ld", data);
	print_s(str);
}
void SerialClass::print(int data)
{
	char str[20];
	sprintf(str, "%d", data);
	print_s(str);
}
void SerialClass::print(float data, uint8_t ndigit)
{
	char str[20];
	char format[6] = "%.0f";
	format[2] = 0x30 + ndigit;
	sprintf(str, format, data);
	print_s(str);
}
void SerialClass::print(double data, uint8_t ndigit)
{
	char str[20];
	char format[6] = "%.0lf";
	format[2] = 0x30 + ndigit;
	sprintf(str, format, data);
	print_s(str);
}
void SerialClass::println(char *data)
{
	print(data);
	print_c('\r');
	print_c('\n');
}
void SerialClass::println(long data)
{
	print(data);
	print_c('\r');
	print_c('\n');
}
void SerialClass::println(int data)
{
	print(data);
	print_c('\r');
	print_c('\n');
}
void SerialClass::println(float data, uint8_t ndigit)
{
	print(data, ndigit);
	print_c('\r');
	print_c('\n');
}
void SerialClass::println(double data, uint8_t ndigit)
{
	print(data, ndigit);
	print_c('\r');
	print_c('\n');
}
uint8_t SerialClass::print_c(char c)
{
	USART_SendData(USART1, c);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
		;

	return 1;
}
void SerialClass::print_s(char* str)
{
	while (*str != '\0')
	{
		print_c(*str);
		str++;
	}
}

/*串口缓冲器的中断程序*/
extern "C" void USART1_IRQHandler(void)
{
	if (USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)
		USART_ReceiveData(USART1);
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		USART1_RX_Buf[USART1_RX_SP] = USART_ReceiveData(USART1);
//		USART1_RX_Buf[USART1_RX_SP] = USART1->DR;
		USART1_RX_SP++;
		USART1_Read_Available++;
//		trace_printf("%d  %d\n", USART1_RX_SP, USART1_Read_Available);
	}
//	USART_SendData(USART1,USART1->DR);
//	trace_printf("%d  %d\n", USART1_RX_SP, USART1_Read_Available);
}

uint8_t SerialClass::available()
{
	return USART1_Read_Available;
}

char SerialClass::read()
{
	USART1_Read_Available--;
	return USART1_RX_Buf[USART1_Read_SP++];
}

void SerialClass::read(char* buf, uint8_t len)
{
	while (len--)
	{
		*buf = read();
		buf++;
	}
}
