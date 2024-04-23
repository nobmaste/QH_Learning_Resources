#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "AD9854.h"
#include "tim3.h"
u16 TIM_FLAG=0;

//    AM中需要用到的定时器		//
void VVInit(void)                                              
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_TimeBaseStructure.TIM_Period = 9999;
	TIM_TimeBaseStructure.TIM_Prescaler=7200;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	

}
void tim_set (uint32_t fre_out, uint32_t tim_clk)						
{
	uint16_t N1, N2;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	N1 = (uint16_t)(((float)tim_clk / fre_out) / 65536);
	N2 = (float)tim_clk / ((N1 + 1) * fre_out);
	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_TimeBaseStructure.TIM_Period =N2-1;
	TIM_TimeBaseStructure.TIM_Prescaler=N1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM3, ENABLE); 

}
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  // 设置NVIC中断分组2，其中2位抢占优先级，2位响应优先级
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);               
  // TIM1中断
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;		 
	// 先占优先级0级
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	
	// 从优先级0级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
	// 使能TIM3中断
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								
  NVIC_Init(&NVIC_InitStructure);
}

void TIM3_IRQHandler(void)   
{
	
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)     //检查指定的TIM中断发生更新中断
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);          //清除TIMx的更新中断标志位	
		TIM_FLAG = 1;
		
	}
}
void TIM3_Init(void)
{
	VVInit();
	NVIC_Configuration();
}
