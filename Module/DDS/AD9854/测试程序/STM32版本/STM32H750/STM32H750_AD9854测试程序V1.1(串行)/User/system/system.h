//-----------------------------------------------------------------
// 程序描述:
//    	设置系统时钟和数据类型定义头文件
// 作    者: 凌智电子
// 开始日期: 2020-11-11
// 完成日期: 2020-11-11
// 当前版本: V1.0
// 历史版本:
//   - V1.0: (2020-11-11)设置系统时钟和MPU内存保护
// 调试工具: 凌智STM32H750核心板、LZE_ST_LINK2
// 说    明: 
//
//-----------------------------------------------------------------
#ifndef _SYSTEM_H
#define _SYSTEM_H
#include "stm32h7xx.h"
//-----------------------------------------------------------------
// 定义一些常用的数据类型短关键字 
//-----------------------------------------------------------------
typedef int32_t  				s32;
typedef int16_t 				s16;
typedef int8_t  				s8;

typedef const int32_t 	sc32;  
typedef const int16_t 	sc16;  
typedef const int8_t 		sc8;  

typedef __IO int32_t  	vs32;
typedef __IO int16_t  	vs16;
typedef __IO int8_t   	vs8;

typedef __I int32_t 		vsc32;  
typedef __I int16_t 		vsc16; 
typedef __I int8_t 			vsc8;   

typedef uint32_t  			u32;
typedef uint16_t 				u16;
typedef uint8_t  				u8;

typedef const uint32_t 	uc32;  
typedef const uint16_t 	uc16;  
typedef const uint8_t 	uc8; 

typedef __IO uint32_t  	vu32;
typedef __IO uint16_t 	vu16;
typedef __IO uint8_t  	vu8;

typedef __I uint32_t 		vuc32;  
typedef __I uint16_t 		vuc16; 
typedef __I uint8_t 		vuc8;  

//-----------------------------------------------------------------
// 外部函数声明
//-----------------------------------------------------------------
extern void SystemClock_Config(void);// 时钟系统配置
extern void CPU_CACHE_Enable(void);
extern void MPU_Set_Protection(u32 Number,u32 BASEADDR,u32 SIZE,u8 AP,u8 DE,u8 S,u8 C,u8 B);
extern void MPU_Memory_Protection(void);
#endif

//-----------------------------------------------------------------
// End Of File
//----------------------------------------------------------------- 
