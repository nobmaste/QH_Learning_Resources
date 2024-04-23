
#ifndef _MB1504_H
#define _MB1504_H
#define uchar unsigned char
#define uint unsigned int
//-----------------------------------------------------------------
// 位操作HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4, GPIO_PIN_SET)
//-----------------------------------------------------------------
#define CLK_0 		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4, GPIO_PIN_RESET)
#define CLK_1 		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4, GPIO_PIN_SET)
#define DATA_0	  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5, GPIO_PIN_RESET)
#define DATA_1    HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5, GPIO_PIN_SET)
#define LE_0		  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_4, GPIO_PIN_RESET)
#define LE_1      HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7, GPIO_PIN_SET)

//-----------------------------------------------------------------
// 外部函数声明
//-----------------------------------------------------------------
extern void GPIO_LED_Configuration(void);
extern void Send16Bit(uint SendData);
extern void Send11Bit( uint SendData ) ;  
extern void Send8Bit( uchar SendData ) ; 
extern void SendReferF( void )  ;
extern void FrequenceToSend( uint FrequenceD ); 
void GPIO_MB1504_Configuration(void);
#endif
