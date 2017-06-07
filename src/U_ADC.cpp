/*
 * U_ADC.cpp
 *
 *  Created on: 2017年6月4日
 *      Author: Romeli
 */

#include <U_ADC.h>

uint16_t U_ADC::Data = 0;

void U_ADC::Init() {
	ADCInit();
}

void U_ADC::RegularChannelConfig(uint8_t ADC_Channel, uint8_t ADC_SampleTime) {
	ADC_ChannelConfig(ADC1, ADC_Channel, ADC_SampleTime);
}

void U_ADC::RefreshData() {
	ADC_StartOfConversion(ADC1);
	ADC1->CR |= ((uint32_t) 0x00000004);
	while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == (uint8_t) RESET)
		;
	Data = ADC1->DR;
}

void U_ADC::RefreshData(uint8_t OverLevel) {
	uint32_t sum = 0;
	for (uint16_t i = 0; i < (1 << OverLevel << OverLevel); ++i) {
		RefreshData();
		sum += Data;
	}
	Data = sum >> OverLevel;
}

void U_ADC::RefreshData(uint8_t ADC_Channel, uint8_t ADC_SampleTime,
		uint8_t OverLevel) {
	RegularChannelConfig(ADC_Channel, ADC_SampleTime);
	RefreshData(OverLevel);
}

void U_ADC::ADCInit() {
	ADC_InitTypeDef ADC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	ADC_DeInit(ADC1);
	ADC_StructInit(&ADC_InitStructure);
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_Init(ADC1, &ADC_InitStructure);

	RCC_ADCCLKConfig(RCC_ADCCLK_PCLK_Div4);

	ADC_ChannelConfig(ADC1, ADC_Channel_0, ADC_SampleTime_239_5Cycles);

	ADC_TempSensorCmd(ENABLE);

	ADC_GetCalibrationFactor(ADC1);

	ADC_Cmd(ADC1, ENABLE);

	while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN))
		;
}
