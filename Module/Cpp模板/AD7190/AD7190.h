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
 *@Attention
 *引脚连接方式
 *SCLK 			PB3
 *CS			PB7
 *DOUT 			PB5 	
 *DIN  			PB6		接收端
 **********************************************************************
 *@Compatibility	加入了对延时的初始化，在与其他需要delay功能的模块
 *					共用时有可能出现问题
 *
 *整体兼容性：中
 *********************************************************************
 *@Characteristic	SPI采用软件搭建，在初始化的过程中使用了延时
 *					时间不足没有封装每一位配置指令，如果需要改变
 *					寄存器配置请参考芯片手册
 *********************************************************************/
#ifndef	__AD7190_H
#define __AD7190_H
extern "C"{
	#include "stm32f4xx_gpio.h"
	#include "sys.h"
	#include "delay.h"
}

#define	 AD7190_ACC        	16777216
#define  SCLK    			PBout(3)
#define  DOUT   			PBout(5)
#define  DIN    			PBin(6) 
#define  CS     			PBout(7)    

#define	RCC_CS1				RCC_AHB1Periph_GPIOB
#define	RCC_SPI_CLK			RCC_AHB1Periph_GPIOB
#define	RCC_SPI_DIN			RCC_AHB1Periph_GPIOB
#define	RCC_SPI_DOUT		RCC_AHB1Periph_GPIOB

#define PIN_CS1        		GPIO_Pin_7
#define PIN_SPI_CLK    		GPIO_Pin_3
#define PIN_SPI_DIN    		GPIO_Pin_6
#define PIN_SPI_DOUT   		GPIO_Pin_5

#define GPIO_CS1       		GPIOB
#define GPIO_SPI_CLK   		GPIOB
#define GPIO_SPI_DIN      	GPIOB
#define GPIO_SPI_DOUT  		GPIOB

class AD7190
{
public:
	AD7190();
	~AD7190();
//用户接口
public:
	void AD7190_Init(void);
	//读取转换前后的电压值
	u32 Get_Register_Val(void);
	double Get_Voltage(void);
//配置函数
private:
	void IO_Init(void);
	void data_write(u8 data);
	u8 data_read(void);
	//读取AD7190数据
	u32 AD7190_Read_Data(u16 ID);		
};

#endif
