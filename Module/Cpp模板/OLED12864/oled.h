/*
 **********************************************************************
 *Copyright(c)		Lance Zhang
 *Identification	CHIC
 *Describtion		Template
 *
 *Present Version	1.0
 *Writer			Lance Zhang
 *Date				8/1/2018
 **********************************************************************/
/**
 **********************************************************************
 *@Attention
 *		delay.h
 *		stm32f4xx_gpio.h
 *@PinPort:
 *		DO --------->	PB8
 *		DI --------->	PB9
 *		RES -------->	PB6
 *		DC --------->	PB3
 *		CS --------->	PB7
 **********************************************************************
 *可能产生的冲突：
 *				暂无
 **********************************************************************
**/
#ifndef __OLED_H
#define __OLED_H
extern "C"{
#include "sys.h"
#include "stdlib.h"		
}
    
//////////////////////////////////////////////////////////////////////////////////	 
 
////////////////////////////////////////////////////////////////////////////////// 	  
 

//OLED模式设置
//0: SPI模式  （模块的BS1，BS2均接GND）
//1: 并行8080模式 （模块的BS1，BS2均接VCC）
//2: I2C模式 （模块的BS1，BS2均接VCC）
#define SPI		0
#define B8080	1
#define I2C		2

#define OLED_MODE  SPI 

#define FONT_LARGE		24
#define FONT_MID		16
#define FONT_SMALL		12
//-----------------OLED端口定义---------------- 
#define OLED_CS_CLOCK     RCC_AHB1Periph_GPIOB
#define OLED_RST_CLOCK    RCC_AHB1Periph_GPIOB
#define OLED_RS_CLOCK     RCC_AHB1Periph_GPIOB
#define OLED_WR_CLOCK     RCC_AHB1Periph_GPIOA
#define OLED_RD_CLOCK     RCC_AHB1Periph_GPIOD
#define OLED_CS 	PBout(7)				  //CS
#define OLED_RST  PBout(6)					  //RESET
#define OLED_RS 	PBout(3)				  //DC
#define OLED_WR 	PAout(4)		  		  
#define OLED_RD 	PDout(7)
#define OLED_CS_GPIO     GPIOB
#define OLED_CS_PIN      GPIO_Pin_7
#define OLED_RST_GPIO    GPIOB
#define OLED_RST_PIN     GPIO_Pin_6
#define OLED_RS_GPIO     GPIOB
#define OLED_RS_PIN      GPIO_Pin_3
#define OLED_WR_GPIO     GPIOA
#define OLED_WR_PIN      GPIO_Pin_4
#define OLED_RD_GPIO     GPIOD
#define OLED_RD_PIN      GPIO_Pin_7
/******************************SPI 模式**************************************/
#ifdef	SPI
	#define OLED_SCLK_CLOCK    RCC_AHB1Periph_GPIOB
	#define OLED_SDIN_CLOCK    RCC_AHB1Periph_GPIOB
	#define OLED_SCLK 	       PBout(8)			  //CLK.D0
	#define OLED_SDIN 	       PBout(9)			  //D1
	#define OLED_SCLK_GPIO     GPIOB
	#define OLED_SCLK_PIN      GPIO_Pin_8
	#define OLED_SDIN_GPIO     GPIOB
	#define OLED_SDIN_PIN      GPIO_Pin_9
#endif
/******************************I2C 模式**************************************/
#ifdef	I2C
	#define SDA_IN()  {GPIOC->MODER&=~(3<<(7*2));GPIOC->MODER|=0<<7*2;}	//PB9输入模式
	#define SDA_OUT() {GPIOC->MODER&=~(3<<(7*2));GPIOC->MODER|=1<<7*2;} //PB9输出模式
	#define OLED_SCL_CLOCK    RCC_AHB1Periph_GPIOC
	#define OLED_SDA_CLOCK    RCC_AHB1Periph_GPIOC
	#define OLED_SCL 	        PCout(6)
	#define OLED_SDA 	        PCout(7)
	#define READ_SDA   PCin(7)  //输入SDA 
	#define OLED_SCL_GPIO     GPIOC
	#define OLED_SCL_PIN      GPIO_Pin_6
	#define OLED_SDA_GPIO     GPIOC
	#define OLED_SDA_PIN      GPIO_Pin_7

	#define OLED_CMD  	0		//写命令
	#define OLED_DATA 	1		//写数据
#endif

class OLED12864{

public:
	OLED12864();
	~OLED12864();
	//OLED控制用函数
	void OLED_WR_Byte(u8 dat,u8 cmd);	    
	void OLED_Display_On(void);
	void OLED_Display_Off(void);
	void OLED_Refresh_Gram(void);		   
											
	void OLED_Init(void);
	void OLED_Clear(void);
	void OLED_DrawPoint(u8 x,u8 y,u8 t);
	void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
	void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);
	void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
	void OLED_ShowString(u8 x,u8 y,const char *p,u8 size);	 

private:
	//OLED初始化协议
	void IIC_Init(void);                					//初始化IIC的IO口				 
	void IIC_Start(void);									//发送IIC开始信号
	void IIC_Stop(void);	  								//发送IIC停止信号
	void IIC_Send_Byte(u8 txd);								//IIC发送一个字节
	u8 	 IIC_Read_Byte(unsigned char ack);					//IIC读取一个字节
	u8   IIC_Wait_Ack(void); 								//IIC等待ACK信号
	void IIC_Ack(void);										//IIC发送ACK信号
	void IIC_NAck(void);									//IIC不发送ACK信号
	void Write_IIC_Command(unsigned char IIC_Command);
	void Write_IIC_Data(unsigned char IIC_Data);

protected:
	void OLED_Data_Out(u8 data);
	u32  mypow(u8 m,u8 n);
};

#endif

