/*
 * U_ADC.h
 *
 *  Created on: 2017年6月4日
 *      Author: Romeli
 */

#ifndef SRC_UADC_H_
#define SRC_UADC_H_

#include "cmsis_device.h"

#define U_ADC_Max 4095

class U_ADC {
public:
	static uint16_t Data;
	static void Init();
	static void RegularChannelConfig(uint8_t ADC_Channel,
			uint8_t ADC_SampleTime);
	static void RefreshData();
	static void RefreshData(uint8_t OverLevel);
	static void RefreshData(uint8_t ADC_Channel, uint8_t ADC_SampleTime,
			uint8_t OverLevel = 0);
private:
	static void GPIOInit();
	static void ADCInit();
};

#endif /* SRC_UADC_H_ */
