#include "MAX262.h"

MAX262::MAX262()
{
	IO_Init();
	//用来给数据读写做延时，这个不能去掉
	delay_init(168);
	//初始化心跳，只是初始化，到这一步为止没有心跳产生
	HeartBeat_Init();
}

MAX262::~MAX262()
{
	
}

void MAX262::HeartBeat_Init()
{
	//自动重装初值：84
	arr = 84 - 1;
	//时钟预分频系数：0（不分频）
	psc = 1 - 1;
	//产生的PWM频率：（定时器主频/预分频系数）/自动重装初值
	//84/1 = 84MHz； 84MHz/84 = 1MHz；
	TIMx			= 	TIM14;
	TIM_CLKBUS 		= 	RCC_APB1Periph_TIM14;
	
	GPIO_CLKBUS 	= 	RCC_AHB1Periph_GPIOA;
	GPIOx 			= 	GPIOA;
	GPIO_PinSource 	= 	GPIO_PinSource7;
	GPIO_AF_TIMx 	= 	GPIO_AF_TIM14;
	GPIO_Pin_x 		= 	GPIO_Pin_7;
/************************************************************************/
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	//TIM14时钟使能    
	RCC_APB1PeriphClockCmd(TIM_CLKBUS,ENABLE);  		
	//使能PORTF时钟		
	RCC_AHB1PeriphClockCmd(GPIO_CLKBUS, ENABLE); 			
	//GPIOF9复用为定时器14
	GPIO_PinAFConfig(GPIOx,GPIO_PinSource,GPIO_AF_TIMx); 			
	//GPIO A7
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

void MAX262::IO_Init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	GPIO_InitStructure.GPIO_Pin = EN_Pin ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

void MAX262::Data_Out(u8 data)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	GPIO_InitStructure.GPIO_Pin = ADDR_Pin_0 | ADDR_Pin_1 | ADDR_Pin_2 | 
									ADDR_Pin_3 | DATA_Pin_0 | DATA_Pin_1 | EN_Pin ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIOE->ODR &= ~(0x3f<<8);
	GPIOE->ODR |= data << 8;		
} 

u8 MAX262::Data_In(void)
{ 
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	GPIO_InitStructure.GPIO_Pin = ADDR_Pin_0 | ADDR_Pin_1 | ADDR_Pin_2 | 
									ADDR_Pin_3 | DATA_Pin_0 | DATA_Pin_1 | EN_Pin ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	u8 dat;
	dat=(GPIOE->IDR) >> 8;
	return dat;
}

void MAX262::write_data(u8 addr, u8 data)
{	
	//data&0x03 ： 1111 -> 0011,不影响PE14和PE15
	Data_Out(addr|((data&0x03)<<4)); 
	EN = 0;
	delay_us(100);
	EN = 1;
	delay_us(50);
}

void MAX262::Set_Af(u8 data_f)
{
	write_data(1,data_f);  
	data_f = data_f>>2;         
	write_data(2,data_f);
	data_f = data_f>>2;         
	write_data(3,data_f);
}

//设置A通道Q值
//7位品质因数Q控制位Q0-Q6 对应十进制0-127
void MAX262::Set_Aq(u8 data_q)    
{    
	write_data(4,data_q);         
	data_q = data_q>>2;         
	write_data(5,data_q);         
	data_q = data_q>>2;         
	write_data(6,data_q);    
	//对地址7只取一位 
	data_q = (data_q>>2)&1;       
	write_data(7,data_q);    
} 

//设置B通道F值     
void MAX262::Set_Bf(u8 data_f)    
{    
    write_data(9,data_f);        
	data_f = data_f>>2;       
	write_data(10,data_f);      
	data_f = data_f>>2;       
	write_data(11,data_f);    
} 
    
//设置B通道Q值     
void MAX262::Set_Bq(u8 data_q)    
{    
    write_data(12,data_q);         
	data_q = data_q>>2;        
	write_data(13,data_q);         
	data_q = data_q>>2;         
	write_data(14,data_q);    
    data_q = (data_q>>2)&1;
	write_data(15,data_q);    
}

//根据f0、Q、workMode和通道channel设置MAX262的陷波工作数据 
//返回时钟频率fclk 
float MAX262::bs_WorkFclk(float f0,float Q,u8 workMode,u8 channel) 
{ 
	u8 Qn; 
    if (workMode==modeTwo)              
	{
		//强制单精度计算，提高效率
		Qn = (u8)(128-90.51f/Q);          
	}
	else
	{		
    Qn = (u8)(128-64/Q); 
	}		
	if(channel==channelA)		
	{ 
        write_data(0,workMode);       
		Set_Af(63);
		//要想获得大范围可调，开关电容的采样频率参数取最大值.     		
		Set_Aq(Qn);       
	}     
	else      
	{ 
		write_data(8,workMode);         
		Set_Bf(63);     
		//要想获得大范围可调，开关电容的采样频率参数取最大值.         
		Set_Bq(Qn);       
	}   
	if(workMode==modeTwo)        
	{
		return  31.46625*PI*f0;     
	}
	else
	{		
       return  44.5*PI*f0; 
	}
}
//心跳函数
void MAX262::CLK_set(float freq)
{
	u32 arr_t;
	
	if(freq<100000)
	{
		TIM14->PSC = 840-1;
		arr_t = 100000/freq;
		arr_t--;
		TIM14->ARR = arr_t;
		TIM14->CCR1 = arr_t/2;
	}
	else
	{
		TIM14->PSC = 0;
		arr_t = 84000000/freq;
		arr_t--;
		TIM14->ARR = arr_t;
		TIM14->CCR1 = arr_t/2;
	}
}

void MAX262::Filter_On(float f0,float Q,u8 workMode,u8 channel)
{
	float heartbeat;
	heartbeat = bs_WorkFclk(f0,Q,workMode,channel);
	CLK_set(heartbeat);
}
