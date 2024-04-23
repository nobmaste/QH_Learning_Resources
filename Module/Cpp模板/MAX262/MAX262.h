/*
 **********************************************************************
 *Copyright(c)		Lance Zhang
 *Identification	CHIC
 *Describtion		Template
 *
 *Present Version	1.0
 *Writer			Lance Zhang
 *Date				7/19/2019
 **********************************************************************
**/
/*
 **********************************************************************
 *需要单片机提供心跳，心跳时钟主频设置为84MHz，位于TIM14 CH1，
 *所需库：
 *		stm32f4xx_gpio.h
 *		stm32f4xx_tim.h
 **********************************************************************
 *可能产生的冲突：
 *				暂无
 **********************************************************************
 *使用方法：
 *		导入需要导入的文件(MAX262.c | MAX262.h)
 *		初始化类 --- MAX262
 *		调用 Filter_On()函数启动带阻滤波
 **********************************************************************
**/
#ifndef	__MAX262_H
#define __MAX262_H
extern "C"
{
	#include "stm32f4xx_gpio.h"
	#include "stm32f4xx_tim.h"
	#include "delay.h"
	#include "sys.h"
}
#define MAX262_Port		GPIOE
#define MAX262_IOCLK	
//[A0 : A3]
#define ADDR_Pin_0		GPIO_Pin_8
#define ADDR_Pin_1		GPIO_Pin_9
#define ADDR_Pin_2		GPIO_Pin_10
#define ADDR_Pin_3		GPIO_Pin_11
//[D0 D1]
#define DATA_Pin_0		GPIO_Pin_12
#define DATA_Pin_1		GPIO_Pin_13
//锁存器使能引脚
#define EN_Pin			GPIO_Pin_14	

#define EN		PEout(14)

#define PI	3.1415926
extern enum 
{
	modeOne=0,
	modeTwo,
	modeThree,
	modeFour
}workMode;

extern enum 
{
	lowPass=0,
	highPass,
	bandPass,
	allPass,
	bandStop
}passMode; 

extern enum 
{
	channelA=0,
	channelB
}channel; 

//CLK时钟位于PA7，频率 1MHz
class	MAX262
{
public:
	MAX262();
	~MAX262();
	float bs_WorkFclk(float f0 = 5000,float Q = 2,u8 workMode = modeOne,u8 channel = channelA);
	void Filter_On(float f0 = 5000,float Q = 2,u8 workMode = modeOne,u8 channel = channelA);
private:
	void IO_Init();
	void write_data(u8 addr, u8 data);
	void Data_Out(u8 data);
	u8	Data_In(void);
	void Set_Af(u8 data_f);
	void Set_Aq(u8 data_q);
	void Set_Bf(u8 data_f);
	void Set_Bq(u8 data_q);
//芯片心脏
private:
	void HeartBeat_Init();	
	void CLK_set(float freq);

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
