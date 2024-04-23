#include "COUNTER.h"

COUNTER::COUNTER()
{
	//设置系统中断优先级分组2
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);				
	//TIM3_Int_Init();	
	//LED_Init();
}

COUNTER::~COUNTER()
{

}
/*TIM3初始化	自动装载初值设定：5000 - 1，
				定时器分频：8400 - 1
				定时器时钟：84MHz；
*/
void COUNTER::TIM3_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	//使能TIM3时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  		
	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Period = arr; 				
	//定时器分频
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  					
	//向上计数模式
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; 
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	//初始化TIM3
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStructure);
	//允许定时器3更新中断
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); 
	//使能定时器3
	TIM_Cmd(TIM3,ENABLE); 
	
	//定时器3中断
	NVIC_InitStructure.NVIC_IRQChannel=TIM3_IRQn; 
	//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; 
	//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x03; 
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);	
}
/*LED初始化，调试用*/
void COUNTER::LED_Init(void)
{    	 
  GPIO_InitTypeDef  GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟

  //GPIOF9,F10初始化设置
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;//LED2和LED3对应IO口
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO

  GPIO_ResetBits(GPIOA,GPIO_Pin_5);//设置高，灯灭

}
