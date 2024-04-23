#include "Capture.h"

Capture::Capture()
{
	arr = 0XFFFFFFFF;
	psc = 84-1;
	Capture_Init(psc,arr);
}

Capture::~Capture()
{
	
}

void Capture::Capture_Init(u32 psc,u16 arr)
{
	GPIO_InitTypeDef 			GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  	TIM_TimeBaseStructure;
	NVIC_InitTypeDef 			NVIC_InitStructure;

	//TIM5时钟使能    
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  	
	//使能PORTA时钟	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	
	//GPIOA0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
	//复用功能
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	//速度100MHz
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	//推挽复用输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	//下拉
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; 
	//初始化PA0
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	//PA0复用位定时器5
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0,GPIO_AF_TIM5); 
	//定时器分频，输入为0xffffffff时为：1us
	TIM_TimeBaseStructure.TIM_Prescaler = psc;  
	//向上计数模式
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	//自动重装载值
	TIM_TimeBaseStructure.TIM_Period = arr;   
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);
	
	//初始化TIM5输入捕获参数
	TIM_ICInitTypeDef  				TIM5_ICInitStructure;
	//CC1S=01 	选择输入端 IC1映射到TI1上
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_1; 
	//上升沿捕获
	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	
	//映射到TI1上
	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 
	//配置输入分频,不分频 
	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 
	//IC1F=0000 配置输入滤波器 不滤波
	TIM5_ICInitStructure.TIM_ICFilter = 0x00;
	TIM_ICInit(TIM5, &TIM5_ICInitStructure);
	//允许更新中断 ,允许CC1IE捕获中断	
	TIM_ITConfig(TIM5,TIM_IT_Update|TIM_IT_CC1,ENABLE);
	//使能定时器5
	TIM_Cmd(TIM5,ENABLE ); 		


	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;
	//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		
	//IRQ通道使能
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	//根据指定的参数初始化VIC寄存器、
	NVIC_Init(&NVIC_InitStructure);	
}
