#include "PWM.h"

PWM::PWM()
{
	//定时器主频设置为：84MHz
	delay_init(84);
	//自动重装初值：500
	arr = 500 - 1;
	//时钟预分频系数：84
	psc = 84 - 1;
	//产生的PWM频率：（定时器主频/预分频系数）/自动重装初值
	//84/84 = 1MHz； 1MHz/500 = 2KHz；
	TIMx			= 	TIM14;
	TIM_CLKBUS 		= 	RCC_APB1Periph_TIM14;
	
	GPIO_CLKBUS 	= 	RCC_AHB1Periph_GPIOA;
	GPIOx 			= 	GPIOA;
	GPIO_PinSource 	= 	GPIO_PinSource7;
	GPIO_AF_TIMx 	= 	GPIO_AF_TIM14;
	GPIO_Pin_x 		= 	GPIO_Pin_7;
}

PWM::~PWM()
{
	
}

void PWM::compare_set(TIM_TypeDef * TIMx,u16 pst)
{
	if(pst >= arr )
		TIM_SetCompare1(TIMx, arr/2);
	else
		TIM_SetCompare1(TIMx, pst);
}

void PWM::pwm_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	//TIM14时钟使能    
	RCC_APB1PeriphClockCmd(TIM_CLKBUS,ENABLE);  		
	//使能PORTF时钟		
	RCC_AHB1PeriphClockCmd(GPIO_CLKBUS, ENABLE); 			
	//GPIOF9复用为定时器14
	GPIO_PinAFConfig(GPIOx,GPIO_PinSource,GPIO_AF_TIMx); 			
	//GPIO A9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_x;
	//复用功能	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        	
	//速度100MHz
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		
	//推挽复用输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;    
	//上拉
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	//初始化PF9
	GPIO_Init(GPIOx,&GPIO_InitStructure);              				
	//定时器分频
	TIM_TimeBaseStructure.TIM_Prescaler = psc;  		
	//向上计数模式
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	
	//自动重装载值	
	TIM_TimeBaseStructure.TIM_Period = arr;   						
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	//初始化定时器14
	TIM_TimeBaseInit(TIMx,&TIM_TimeBaseStructure);					
	
	//初始化TIM14 Channel1 PWM模式	
	//比较输出使能	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	//选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	//输出极性:TIM输出比较极性低
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	//根据T指定的参数初始化外设TIM1 4OC1	
	TIM_OC1Init(TIMx, &TIM_OCInitStructure); 
	//使能TIM14在CCR1上的预装载寄存器	
	TIM_OC1PreloadConfig(TIMx, TIM_OCPreload_Enable);
	//ARPE使能 
	TIM_ARRPreloadConfig(TIMx,ENABLE);								
	//使能TIM14
	TIM_Cmd(TIMx, ENABLE);  																	  	
}
