/*
 * Delay.cpp
 *
 *  Created on: 2016Äê2ÔÂ6ÈÕ
 *      Author: Romeli
 */

#include "Delay.h"

static volatile uint64_t nTime_now = 0;
uint64_t nTime_last;


void SysTick_Init()
{
	SysTick_Config((SystemCoreClock / 1000) - 12);	//Set SysTick timer=us
//	SysTick->CTRL&=SysTick_CTRL_ENABLE_Msk;			//Disable SysTick
}

void Delay_ms(uint32_t ms)
{
//	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk;			//Ensable SysTick
	nTime_last = nTime_now;							//Record time_now
	while ((nTime_now - nTime_last) < ms)
		;				//keep loop when in delay time
}

uint32_t millis()
{
	return nTime_now;
}

uint64_t micros()
{
	return nTime_now * 1000;
}

extern "C" void SysTick_Handler()
{
	nTime_now++;
}

