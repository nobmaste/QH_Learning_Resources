/**
 **********************************************************************
 *@Copyright(c)		Lance Zhang
 *@Identification	CHIC
 *@Describtion		Template
 *
 *@PresentVersion	1.0
 *@Writer			Lance Zhang
 *@Date				7/20/2019
 **********************************************************************/
/**********************************************************************
 *@Instruction
 *使用方法：
 *
 *1、初始化debug_timer类
 *2、调用TIM_SetCounter(TIMx,0);设置计数器初值
 *3、调用TIM_GetCounter(TIM6);	获取计数器当前的计数值
 **********************************************************************/
/**********************************************************************
 *@Attention		每次读取完计数值之后记得重装初值
 *@Attention		默认的定时器初始化为arr = 5000,psc = 8400即500ms
 **********************************************************************
 *@Compatibility	调用了stm32f4xx_tim.h对通用定时器TIM6进行了初始化
 *					计数器为硬件实现，没有包含interrupt.c
 *
 *整体兼容性：高
 *
 *@Characteristic	硬件计数，一般情况下不会被中断服务函数打断
 *					目前已知能够打断计数的服务函数只有复位
 **********************************************************************/

#ifndef	_DEBUG_TIMER
#define _DEBUG_TIMER
extern "C"{
	#include "stm32f4xx_tim.h"
}

class debug_timer
{
public:
	//小bug，无法设置65535作为arr-1，一定要最大的话根据需要把u16改成u32就行了
	debug_timer(u16 arr = 5000,u16 psc = 8400);
	~debug_timer();

	void timer_init(u16 arr,u16 psc);
	
	u32 arr;
	u16 psc;
};

#endif
