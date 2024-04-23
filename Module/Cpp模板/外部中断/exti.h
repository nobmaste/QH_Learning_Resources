/*
 **********************************************************************
 *Copyright(c)		Lance Zhang
 *Identification	CHIC
 *Describtion		Template
 *
 *Present Version	1.0
 *Writer			Lance Zhang
 *Date				7/08/2019
 **********************************************************************
**/
/********************************************************************************
*外部中断封装
*使用时需要包含的库：
*					stm32f4xx_gpio.h
*					stm32f4xx_syscfg.h
*					stm32f4xx_exti.h
*********************************************************************************
*HARDWARE文件：exti.c | exti.h
*对外部中断进行初始化前，请先初始化delay()
********************************************************************************/
#ifndef __EXTI_H
#define __EXIT_H	 
#include "sys.h"
#include "delay.h" 

void EXTIX_Init(void);	//外部中断初始化
void LED_Init(void);
void EXIO_Init(void);

#endif

























