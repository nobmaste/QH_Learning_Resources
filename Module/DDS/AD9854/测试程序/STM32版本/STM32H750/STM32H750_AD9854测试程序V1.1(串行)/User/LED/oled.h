//-----------------------------------------------------------------
// 程序描述:
// 		 OLED驱动头文件
// 作    者: 凌智电子
// 开始日期: 2018-08-04
// 完成日期: 2018-08-04
// 修改日期: 
// 当前版本: V1.0
// 历史版本:
//  - V1.0: (2018-08-04)	
// 调试工具: 凌智STM32F429+Cyclone IV电子系统设计开发板、LZE_ST_LINK2
// 说    明: 
//    
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// 头文件包含
//-----------------------------------------------------------------
#ifndef _OLED_H
#define _OLED_H
#include "system.h"
#include "stdlib.h"	  
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// 宏定义
//-----------------------------------------------------------------
#define OLED_SCLK_L   HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_RESET)
#define OLED_SCLK_H   HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_SET)
#define OLED_SDIN_L   HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_RESET)
#define OLED_SDIN_H   HAL_GPIO_WritePin(GPIOC,GPIO_PIN_2,GPIO_PIN_SET)

#define OLED_CMD  	0		// 写命令
#define OLED_DATA 	1		// 写数据

#define Max_Column	127
#define Max_Row			7

//-----------------------------------------------------------------
// 函数声明
//-----------------------------------------------------------------
extern void IIC_Delay(void);
extern void IIC_Start(void);
extern void IIC_Stop(void);
extern void IIC_Wait_Ack(void);
extern void IIC_Send_Byte(u8 txd);
extern void IIC_Send_Command(u8 IIC_Command);
extern void IIC_Send_Data(u8 IIC_Data);
extern void OLED_WR_Byte(u8 Data,u8 CMD);
extern void Fill_Picture(u8 Fill_Data);
extern void OLED_Set_Pos(u8 x,u8 y);
extern void OLED_Display_On(void);
extern void OLED_Display_Off(void);
extern void OLED_Clear(void);
extern void OLED_On(void);
extern void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size);
extern u32 oled_pow(u8 m,u8 n);
extern void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2);
extern void OLED_ShowString(u8 x,u8 y,char *chr,u8 Char_Size);
extern void OLED_ShowCHinese(u8 x,u8 y,u8 no);
extern void OLED_Init(void);
#endif
//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
