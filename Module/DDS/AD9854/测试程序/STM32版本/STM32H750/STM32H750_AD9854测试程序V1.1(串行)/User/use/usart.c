//-----------------------------------------------------------------
// 程序描述:
// 		 串口通讯驱动程序
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

//-----------------------------------------------------------------
// 头文件包含
//-----------------------------------------------------------------
#include "usart.h"
#include "delay.h"
#include "string.h"
//-----------------------------------------------------------------

u8 USART_RX_BUF[USART_REC_LEN]; 	// 接收缓冲,最大USART_REC_LEN个字节.
u16 USART_RX_STA=0;       				// 接收状态标记（bit15：接收完成标志  bit14：接收到0x0d   bit13~0：接收到的有效字节数目）
u8 aRxBuffer[RXBUFFERSIZE];				// HAL库使用的串口接收缓冲
UART_HandleTypeDef UART_Handler;  // UART句柄

//-----------------------------------------------------------------
// void uart_init(u32 bound)
//-----------------------------------------------------------------
//
// 函数功能: 串口1初始化
// 入口参数: u32 bound：波特率
// 返回参数: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void uart_init(u32 bound)
{	
	//UART 初始化设置
	UART_Handler.Instance=USARTx;					    			// USART1
	UART_Handler.Init.BaudRate=bound;				    		// 波特率
	UART_Handler.Init.WordLength=UART_WORDLENGTH_8B;// 字长为8位数据格式
	UART_Handler.Init.StopBits=UART_STOPBITS_1;	    // 一个停止位
	UART_Handler.Init.Parity=UART_PARITY_NONE;		  // 无奇偶校验位
	UART_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;// 无硬件流控
	UART_Handler.Init.Mode=UART_MODE_TX_RX;		    	// 收发模式
	HAL_UART_Init(&UART_Handler);					    			// HAL_UART_Init()会使能UART1
	
	// 该函数会开启接收中断：标志位UART_IT_RXNE，并且设置接收缓冲以及接收缓冲接收最大数据量
	HAL_UART_Receive_IT(&UART_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE);
}

//-----------------------------------------------------------------
// void HAL_UART_MspInit(UART_HandleTypeDef *huart)
//-----------------------------------------------------------------
//
// 函数功能: UART底层初始化，时钟使能，引脚配置，中断配置
// 入口参数: huart:串口句柄
// 返回参数: 无
// 注意事项: 此函数会被HAL_UART_Init()调用
//
//-----------------------------------------------------------------
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef GPIO_Initure;
	
	if(huart->Instance==USARTx)// 如果是串口1，进行串口1 MSP初始化
	{
		USART_RX_GPIO_CLK_ENABLE();
		USART_TX_GPIO_CLK_ENABLE();
		USART_CLK_ENABLE();				 						 // 使能USART1时钟
	
		GPIO_Initure.Pin=USART_TX_PIN;				 // PA9 -> USART1_TX
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		 // 复用推挽输出
		GPIO_Initure.Pull=GPIO_PULLUP;				 // 上拉
		GPIO_Initure.Speed=GPIO_SPEED_FREQ_HIGH;// 高速
		GPIO_Initure.Alternate=USART_TX_AF;		 // 复用为USART1
		HAL_GPIO_Init(USART_TX_GPIO_PORT,&GPIO_Initure);	   // 初始化PA9

		GPIO_Initure.Pin=USART_RX_PIN;				 //PA10 -> USART1_RX
		HAL_GPIO_Init(USART_RX_GPIO_PORT,&GPIO_Initure);	   // 初始化PA10
		
		HAL_NVIC_EnableIRQ(USART_IRQn);			   // 使能USART1中断通道
		HAL_NVIC_SetPriority(USART_IRQn,3,3);  // 抢占优先级3，子优先级3
	}
}

//-----------------------------------------------------------------
// void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//-----------------------------------------------------------------
//
// 函数功能: 串口中断回调函数
// 入口参数: huart:串口句柄
// 返回参数: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart->Instance==USARTx)		// 如果是串口1
	{
		if((USART_RX_STA&0x8000)==0)// 接收未完成
		{
			if(USART_RX_STA&0x4000)		// 接收到了0x0d
			{
				if(aRxBuffer[0]!=0x0a)	// 接收错误,重新开始
					USART_RX_STA=0;
				else 										// 接收完成了 
					USART_RX_STA|=0x8000;	
			}
			else // 还没收到0X0D
			{	
				if(aRxBuffer[0]==0x0d)
					USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=aRxBuffer[0] ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))
						USART_RX_STA=0;			// 接收数据错误,重新开始接收	  
				}		 
			}
		}

	}
}

//-----------------------------------------------------------------
// void USART1_IRQHandler(void) 
//-----------------------------------------------------------------
//
// 函数功能: 串口1中断服务程序
// 入口参数: 无
// 返回参数: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void USART1_IRQHandler(void)                	
{ 
	u32 timeout=0;
	u32 maxDelay=0x1FFFF;
	
	HAL_UART_IRQHandler(&UART_Handler);	// 调用HAL库中断处理公用函数
	
	timeout=0;
  while (HAL_UART_GetState(&UART_Handler) != HAL_UART_STATE_READY)// 等待就绪
	{
		timeout++;	// 超时处理
    if(timeout>maxDelay) 
			break;		
	}
     
	timeout=0;
	// 一次处理完成之后，重新开启中断并设置RxXferCount为1
	while(HAL_UART_Receive_IT(&UART_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE) != HAL_OK)
	{
		timeout++; // 超时处理
		if(timeout>maxDelay) 
			break;	
	}
} 

//-----------------------------------------------------------------
// 加入以下代码,支持printf函数 
//-----------------------------------------------------------------
#if 1
#pragma import(__use_no_semihosting)             
// 标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 
FILE __stdout;  

// 定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 

// 重定义fputc函数 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->ISR&0X40)==0);// 循环发送,直到发送完毕   
	USART1->TDR = (u8) ch;      
	return ch;
}
#endif 

//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
