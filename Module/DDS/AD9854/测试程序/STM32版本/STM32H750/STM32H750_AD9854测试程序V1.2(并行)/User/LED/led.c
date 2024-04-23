//-----------------------------------------------------------------
// 程序描述:
//     LED显示驱动程序
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

//-----------------------------------------------------------------
// 头文件包含
//-----------------------------------------------------------------
#include "led.h"
//-----------------------------------------------------------------
GPIO_TypeDef* GPIO_PORT[LEDn] = {LEDB_GPIO_PORT,LEDG_GPIO_PORT,LEDR_GPIO_PORT};
const uint16_t GPIO_PIN[LEDn] = {LEDB_PIN,LEDG_PIN,LEDR_PIN};
//-----------------------------------------------------------------
// void LED_Init(void)
//-----------------------------------------------------------------
// 
// 函数功能: LED GPIO配置
// 入口参数: 无 
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void LED_Init(void)
{
  BSP_LED_Init(LEDB);	// LED_B(蓝灯)：PE4
	BSP_LED_Init(LEDG);	// LED_G(绿灯)：PE5
	BSP_LED_Init(LEDR);	// LED_R(红灯)：PE6
}

//-----------------------------------------------------------------
// void BSP_LED_Init(Led_TypeDef Led)
//-----------------------------------------------------------------
// 
// 函数功能: 配置指定的LED
// 入口参数: Led：指定要配置的LED（值：LEDB、LEDG、LEDR） 
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void BSP_LED_Init(Led_TypeDef Led)
{
  GPIO_InitTypeDef  GPIO_InitStruct;
  LEDx_GPIO_CLK_ENABLE(Led);												// 开启Led时钟

  GPIO_InitStruct.Pin = GPIO_PIN[Led];							// 配置Led
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;				// 推挽输出
  GPIO_InitStruct.Pull = GPIO_PULLUP;								// 上拉
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;// 高速
  HAL_GPIO_Init(GPIO_PORT[Led], &GPIO_InitStruct);	// 初始化Led
  
  HAL_GPIO_WritePin(GPIO_PORT[Led], GPIO_PIN[Led], GPIO_PIN_SET); // LED置高
}
//-----------------------------------------------------------------
// End Of File
//----------------------------------------------------------------- 
