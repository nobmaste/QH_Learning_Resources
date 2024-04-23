/*
 **********************************************************************
 *Copyright(c)		Lance Zhang
 *Identification	CHIC
 *Describtion		Template
 *
 *Present Version	1.0
 *Writer			Lance Zhang
 *Date				8/16/2018
 **********************************************************************
**/
/*
 **********************************************************************
 *频率和占空比通过arr和psc控制，时钟主频设置为84MHz
 *所需库：
 *		stm32f4xx_gpio.h
 *		stm32f4xx_tim.h
 **********************************************************************
**/
#ifndef __PWM_
#define __PWM_

extern "C"{
	#include "sys.h"
	#include "usart.h"
	#include "delay.h"
};

class PWM
{
	private:
		
	public:
		PWM();
		~PWM();
		void pwm_init(void);								//初始化，在参数配置完成后调用它以获得pwm波
		void compare_set(TIM_TypeDef * TIMx,u16 pst);		//占空比设置函数，在初始化完成后使用

		u32 arr;
		u32 psc;
		
		uint32_t	TIM_CLKBUS;				//设置TIM时钟线
		uint32_t	GPIO_CLKBUS;			//设置GPIO时钟线
		uint32_t	GPIO_PinSource;			//设置开启复用功能的引脚
		uint16_t	GPIO_Pin_x;				//设置对应的GPIO引脚
		uint8_t		GPIO_AF_TIMx;			//设置复用功能对应的定时器

		GPIO_TypeDef *	GPIOx;				//选取对应的GPIO端口
		TIM_TypeDef*	TIMx;				//选取对应的TIM通道
};

#endif
