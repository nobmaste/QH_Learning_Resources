//-----------------------------------------------------------------
// 程序描述:
// 		 LED驱动头文件
// 作    者: 凌智电子
// 开始日期: 2020-11-11
// 完成日期: 2020-11-11
// 修改日期: 
// 当前版本: V1.0
// 历史版本:
//  - V1.0: (2020-11-11)LED IO 配置
// 调试工具: 凌智STM32H750核心板、LZE_ST_LINK2
// 说    明: 
//    
//-----------------------------------------------------------------
#ifndef _LED_H
#define _LED_H
#include "system.h"
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

// 蓝灯
#define LEDB_PIN                                GPIO_PIN_4
#define LEDB_GPIO_PORT                          GPIOE
#define LEDB_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOE_CLK_ENABLE()  
#define LEDB_GPIO_CLK_DISABLE()                 __HAL_RCC_GPIOE_CLK_DISABLE()  
#define LED_B_ON 																HAL_GPIO_WritePin(LEDB_GPIO_PORT,LEDB_PIN,GPIO_PIN_RESET);
#define LED_B_OFF 															HAL_GPIO_WritePin(LEDB_GPIO_PORT,LEDB_PIN,GPIO_PIN_SET);
#define LED_B_Toggle 														HAL_GPIO_TogglePin(LEDB_GPIO_PORT, LEDB_PIN)

// 绿灯
#define LEDG_PIN                                GPIO_PIN_5
#define LEDG_GPIO_PORT                          GPIOE
#define LEDG_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOE_CLK_ENABLE()  
#define LEDG_GPIO_CLK_DISABLE()                 __HAL_RCC_GPIOE_CLK_DISABLE()
#define LED_G_ON 																HAL_GPIO_WritePin(LEDG_GPIO_PORT,LEDG_PIN,GPIO_PIN_RESET);
#define LED_G_OFF 															HAL_GPIO_WritePin(LEDG_GPIO_PORT,LEDG_PIN,GPIO_PIN_SET);
#define LED_G_Toggle 														HAL_GPIO_TogglePin(LEDG_GPIO_PORT, LEDG_PIN)

// 红灯
#define LEDR_PIN                                GPIO_PIN_6
#define LEDR_GPIO_PORT                          GPIOE
#define LEDR_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOE_CLK_ENABLE()  
#define LEDR_GPIO_CLK_DISABLE()                 __HAL_RCC_GPIOE_CLK_DISABLE() 
#define LED_R_ON 																HAL_GPIO_WritePin(LEDR_GPIO_PORT,LEDR_PIN,GPIO_PIN_RESET);
#define LED_R_OFF 															HAL_GPIO_WritePin(LEDR_GPIO_PORT,LEDR_PIN,GPIO_PIN_SET);
#define LED_R_Toggle 														HAL_GPIO_TogglePin(LEDR_GPIO_PORT, LEDR_PIN)

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
