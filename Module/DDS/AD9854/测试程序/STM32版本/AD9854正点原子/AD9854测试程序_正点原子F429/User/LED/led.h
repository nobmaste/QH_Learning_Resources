//-----------------------------------------------------------------
// 程序描述:
// 		 LED驱动头文件
// 作    者: 凌智电子
// 开始日期: 2018-08-04
// 完成日期: 2018-08-04
// 修改日期: 
// 当前版本: V1.0
// 历史版本:
//  - V1.0: (2018-08-04)LED IO 配置
// 调试工具: 凌智STM32F429+Cyclone IV电子系统设计开发板、LZE_ST_LINK2
// 说    明: 
//    
//-----------------------------------------------------------------
#ifndef _LED_H
#define _LED_H
#include "stm32f429_winner.h"
//-----------------------------------------------------------------
// LED引脚定义
//-----------------------------------------------------------------
typedef enum 
{
  LEDB = 0,
  LEDG = 1,
	LEDR = 2
}Led_TypeDef;

#define LEDn                                    3

#define LEDB_PIN                                GPIO_PIN_14
#define LEDB_GPIO_PORT                          GPIOB
#define LEDB_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOB_CLK_ENABLE()  
#define LEDB_GPIO_CLK_DISABLE()                 __HAL_RCC_GPIOB_CLK_DISABLE()  
#define LED_B 																	PBout(14)												// LED_B(蓝灯)		PB14

#define LEDG_PIN                                GPIO_PIN_7
#define LEDG_GPIO_PORT                          GPIOF
#define LEDG_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOF_CLK_ENABLE()  
#define LEDG_GPIO_CLK_DISABLE()                 __HAL_RCC_GPIOF_CLK_DISABLE()
#define LED_G 																	PFout(7)												// LED_G(绿灯)		PF7

#define LEDR_PIN                                GPIO_PIN_3
#define LEDR_GPIO_PORT                          GPIOI
#define LEDR_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOI_CLK_ENABLE()  
#define LEDR_GPIO_CLK_DISABLE()                 __HAL_RCC_GPIOI_CLK_DISABLE() 
#define LED_R 																	PIout(3)												// LED_R(红灯)		PI3

#define LEDx_GPIO_CLK_ENABLE(__INDEX__)  do{if((__INDEX__) == 0) LEDB_GPIO_CLK_ENABLE(); else \
                                            if((__INDEX__) == 1) LEDG_GPIO_CLK_ENABLE(); else \
																						if((__INDEX__) == 2) LEDR_GPIO_CLK_ENABLE(); \
                                            }while(0)
#define LEDx_GPIO_CLK_DISABLE(__INDEX__) do{if((__INDEX__) == 0) LEDB_GPIO_CLK_DISABLE(); else \
                                            if((__INDEX__) == 1) LEDG_GPIO_CLK_DISABLE(); else \
																						if((__INDEX__) == 2) LEDR_GPIO_CLK_DISABLE(); \
                                            }while(0)

//-----------------------------------------------------------------

//-----------------------------------------------------------------
// 外部函数声明
//-----------------------------------------------------------------
extern void LED_Init(void);
extern void BSP_LED_Init(Led_TypeDef Led);
#endif
//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
