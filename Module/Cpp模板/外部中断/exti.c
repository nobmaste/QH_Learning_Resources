#include "exti.h"

//中断服务函数
void EXTI2_IRQHandler(void)
{
	delay_ms(10);	//消抖
	if(PEin(2)==0)	  
	{				 
   PAout(5)=!PAout(5); 
	}		 
	 EXTI_ClearITPendingBit(EXTI_Line2);//清除LINE2上的中断标志位 
}
//LED测试函数，可有可无
void LED_Init()
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);	
	PAout(5) = 0;
}
//外部中断端口选择，这里用的是PE2
void EXIO_Init()
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOE,&GPIO_InitStructure);
}

void EXTIX_Init()
{
	//这个LED初始化仅仅用于测试，使用时可以注释掉
	LED_Init();
	///////////
	NVIC_InitTypeDef   NVIC_InitStructure;
	EXTI_InitTypeDef   EXTI_InitStructure;
	//对应的中断端口
	EXIO_Init();
	//使能SYSCFG时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	//PE2 连接到中断线2
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);
	//配置EXTI_Line2
	EXTI_InitStructure.EXTI_Line = EXTI_Line2;
	//中断事件
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	//下降沿触发
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; 
	//中断线使能
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	//配置
	EXTI_Init(&EXTI_InitStructure);
	
	//外部中断2
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
	//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;
	//子优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
	//使能外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	//配置  
	NVIC_Init(&NVIC_InitStructure);
}












