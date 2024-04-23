//-----------------------------------------------------------------
// 程序描述:
// 		 串口通讯驱动程序头文件
// 作    者: 凌智电子
// 开始日期: 2020-11-11
// 完成日期: 2020-11-11
// 修改日期: 
// 当前版本: V1.0
// 历史版本:
//  - V1.0: (2020-11-11)串口初始化，串口中断
// 调试工具: 凌智STM32H750核心板、LZE_ST_LINK2
// 说    明: 
//    
//-----------------------------------------------------------------
#ifndef _USART_H
#define _USART_H
#include "system.h"
//-----------------------------------------------------------------
// 宏定义和变量声明
//-----------------------------------------------------------------
#define USART_REC_LEN  	200  							// 定义最大接收字节数 200
	  	
extern u8  USART_RX_BUF[USART_REC_LEN];  	// 接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u16 USART_RX_STA;         				 	// 接收状态标记	
extern UART_HandleTypeDef UART_Handler; 	// UART句柄

#define RXBUFFERSIZE   1 									// 缓存大小
extern u8 aRxBuffer[RXBUFFERSIZE];			 	// HAL库USART接收Buffer

//-----------------------------------------------------------------
// USART引脚定义
//-----------------------------------------------------------------
#define USARTx                                		 USART1
#define USART_CLK_ENABLE()                         __HAL_RCC_USART1_CLK_ENABLE()

#define USART_RX_PIN                               GPIO_PIN_10
#define USART_RX_GPIO_PORT                         GPIOA
#define USART_RX_AF                         			 GPIO_AF7_USART1
#define USART_RX_GPIO_CLK_ENABLE()                 __HAL_RCC_GPIOA_CLK_ENABLE()  
#define USART_RX_GPIO_CLK_DISABLE()                __HAL_RCC_GPIOA_CLK_DISABLE()  

#define USART_TX_PIN                               GPIO_PIN_9
#define USART_TX_GPIO_PORT                         GPIOA
#define USART_TX_AF                         			 GPIO_AF7_USART1
#define USART_TX_GPIO_CLK_ENABLE()                 __HAL_RCC_GPIOA_CLK_ENABLE()  
#define USART_TX_GPIO_CLK_DISABLE()                __HAL_RCC_GPIOA_CLK_DISABLE()  

#define USART_IRQn																 USART1_IRQn
//-----------------------------------------------------------------
// 外部函数声明
//-----------------------------------------------------------------
extern void uart_init(u32 bound);
//-----------------------------------------------------------------
#endif
//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
