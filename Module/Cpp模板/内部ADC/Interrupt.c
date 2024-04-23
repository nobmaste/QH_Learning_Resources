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
**中断服务函数文件
 **********************************************************************
 *中断服务函数不能放在.cpp文件中，因此需要额外建立一个.c文件
 **********************************************************************
**/
#include "Interrupt.h"
#include "usart.h"
#include "stm32f4xx_adc.h"

u16 VOLTAGE;
u16 VOL_BUF[1024];
u16 count = 0;
u8 flag = 0;
//定时器3中断服务函数
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)==SET) //溢出中断
	{
		if(flag == 0)
		{
			VOL_BUF[count] = ADC_GetConversionValue(ADC1);
			VOLTAGE = VOL_BUF[count];
			count++;			
		}

		if(count == 1024)
		{
			count = 0;
			//flag = 1;
		}						
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);  //清除中断标志位
}
