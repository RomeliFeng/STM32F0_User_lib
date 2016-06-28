/*
 * PWM.cpp
 *
 *  Created on: 2016��4��30��
 *      Author: Romeli
 */
#include "PWM.h"

PWMClass PWM;

void PWMClass::init(uint16_t Period = 4095, uint16_t Duty = 0)
{
	TIM3_GPIO_Config();
	TIM3_Mode_Config(Period, Duty);
}
void PWMClass::set(uint16_t Duty)
{
	for (uint8_t i = 1; i < 5; i++)
	{
		set(i, Duty);
	}
}
void PWMClass::set(uint8_t ch, uint16_t Duty)
{
	switch (ch)
	{
#if OC1EN
	case 1:
		TIM_SetCompare1(TIM3, Duty);
		break;
#endif
#if OC2EN
		case 2:
		TIM_SetCompare2(TIM3,Duty);
		break;
#endif
#if OC3EN
		case 3:
		TIM_SetCompare3(TIM3,Duty);
		break;
#endif
#if OC4EN
		case 4:
		TIM_SetCompare4(TIM3,Duty);
		break;
#endif
	}
}
void TIM3_GPIO_Config()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//����TIM3ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	//����GPIOAʱ��
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
#if OC1EN
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_1);
#endif
#if OC2EN
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_1);
#endif
#if OC3EN
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_1);
#endif
#if OC4EN
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_1);
#endif

}
void TIM3_Mode_Config(uint16_t Period, uint16_t Duty)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	//��ʼ��TIM
	//���������ֵ
	TIM_TimeBaseStructure.TIM_Period = Period;
	//Ԥ��Ƶϵ��������Ƶ
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	//ʱ�ӷ�Ƶϵ��������Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	//���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	//��ʼ��TIM3
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	//����ģʽΪPWM1
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	//��������ֵΪռ�ձ�
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = Duty;
	//����PWM��ʼΪ�ߵ�ƽ�������Ϊ�͵�ƽ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	//Ĭ��ռ�ձ�һ�¹ʲ�����ռ�ձ��޸�
#if OC1EN
	//ʹ��ͨ��1
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
#endif
#if OC2EN
	//ʹ��ͨ��2
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
#endif
#if OC3EN
	//ʹ��ͨ��3
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
#endif
#if OC4EN
	//ʹ��ͨ��4
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
#endif
	//ʹ��TIM3���ؼĴ���
	TIM_ARRPreloadConfig(TIM3, ENABLE);

	//ʹ�ܶ�ʱ��2
	TIM_Cmd(TIM3, ENABLE);
}
