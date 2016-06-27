/*
 * Delay.h
 *
 *  Created on: 2016Äê2ÔÂ6ÈÕ
 *      Author: Romeli
 */

#ifndef DELAY_H_
#define DELAY_H_

#include "stm32f0xx.h"

extern void SysTick_Init();
extern void Delay_ms(uint32_t ms);

extern inline void Delay_s(uint32_t s)
{
	Delay_ms(s * 1000);
}
extern uint32_t millis();
extern uint64_t micros();

#endif /* DELAY_H_ */
