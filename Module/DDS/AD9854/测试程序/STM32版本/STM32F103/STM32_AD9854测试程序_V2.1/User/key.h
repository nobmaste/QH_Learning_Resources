//-----------------------------------------------------------------
// 独立键盘模块头文件
// 头文件名: key.h
// 作    者: 凌智电子
// 编写时间: 2014-02-01
// 修改日期: 2014-02-02
// 当前版本: V1.0.1 
// 历史版本:
//-----------------------------------------------------------------

#ifndef _KEY_H_
#define _KEY_H_
#include "stm32f10x.h"

#define KEY1 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_0)
#define KEY2 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_1)
#define KEY3 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)
#define KEY4 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)

void Key_Init(void);
u8 Key_Scan(void);


#endif
//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
