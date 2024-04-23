#include "AD9850.h"
#include "sys.h"
#include "delay.h"
/*8080并行out*/
void Data_Out(u8 data)
{
	B8080_Init(GPIO_Mode_OUT);  			//输出模式
	GPIOE->ODR&=~(0XFF<<8);		//清空8~15
	GPIOE->ODR|=data<<8;		//数据传给高8位

	GPIO_Write(GPIOE,data<<8);
} 

/*8080并行In*/
u8 Data_In(void)
{ 
	B8080_Init(GPIO_Mode_IN);  //输入模式
	u8 dat;
 
	dat=(GPIOE->IDR)>>8;
	return dat;
}

// GPIO_Mode_IN   = 0x00, 
// GPIO_Mode_OUT  = 0x01,
void B8080_Init(GPIOMode_TypeDef mode)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15 ;
	GPIO_InitStructure.GPIO_Mode = mode;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//上拉
	GPIO_Init(GPIOE, &GPIO_InitStructure);				//初始化

}

void AD9850_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_WCLK,ENABLE);
	GPIO_InitStructure.GPIO_Pin = PIN_WCLK;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//上拉
	GPIO_Init(GPIO_WCLK, &GPIO_InitStructure);			//初始化

	RCC_AHB1PeriphClockCmd(RCC_FQUP,ENABLE);
	GPIO_InitStructure.GPIO_Pin = PIN_FQUP ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//上拉
	GPIO_Init(GPIO_FQUP, &GPIO_InitStructure);			//初始化

	RCC_AHB1PeriphClockCmd(RCC_REST,ENABLE);
	GPIO_InitStructure.GPIO_Pin =PIN_REST ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;		//上拉
	GPIO_Init(GPIO_REST, &GPIO_InitStructure);			//初始化
	WCLK_L;
	FQUP_L;
	REST_L;
}
//复位
void AD9850_Rest()
{
  WCLK_L;
  FQUP_L;
  REST_L;
  REST_H;
  delay_us(5);
	REST_L;
}

//设置相位，频率(w0中包括5位相位，1位掉电，2位控制)
void AD9850_SetOut(double frequence,u8 w0)
{
  u8 w;
  u32 y,x;
//计算频率的HEX值
  x=4294967295/125;//适合125M晶振
//如果时钟频率不为125MHZ，修改该处的频率值，单位MHz
  frequence=frequence/1000000;
  frequence=frequence*x;
  y=frequence;
	//写w0数据
  w=w0; 
  Data_Out(w0);
	WCLK_H;
	WCLK_L;
	//写w1数据
  w=(y>>24);
	Data_Out(w);
	WCLK_H;
	WCLK_L;
	//写w2数据
  w=(y>>16);
	Data_Out(w);
	WCLK_H;
	WCLK_L;
	//写w3数据
  w=(y>>8);
	Data_Out(w);
	WCLK_H;
	WCLK_L;
	//写w4数据
  w=y;
	Data_Out(w);
	WCLK_H;
	WCLK_L;
	//移入始能
	FQUP_L;
	FQUP_H;
	delay_us(5);
	FQUP_L;
	//Data_Out(y<<8);
}
