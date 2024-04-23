#include "MB1504.h"
#include "main.h"
#include "Delay.h"
//-----------------------------------------------------------------
// 初始化程序区
//-----------------------------------------------------------------
//-----------------------------------------------------------------
// void GPIO_MB1504_Configuration(void)
//-----------------------------------------------------------------
// 
// 函数功能: MB1504 GPIO配置
// 入口参数: 无 
// 返 回 值: 无
// 全局变量: 无
// 调用模块: RCC_APB2PeriphClockCmd();GPIO_Init();
// 注意事项: 无
//
//-----------------------------------------------------------------
	

void GPIO_MB1504_Configuration(void)
{
	 GPIO_InitTypeDef GPIO_Initure;
    
    /* enable the clock */
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /* set gpio */
    GPIO_Initure.Pin = GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_7;
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Initure.Pull = GPIO_PULLUP;
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_Initure);

}

//-----------------------------------------------------------------
//Send16Bit(uint SendData)	
//-----------------------------------------------------------------
//
// 函数功能: 基准分频数据写入
// 入口参数: SendData?
// 返回参数: 无 
// 全局变量: 无
// 调用模块: 
// 注意事项: 
//-----------------------------------------------------------------
void Send16Bit(uint SendData)												//基准分频  写入16位数据
{
	uchar i;
	uint SendMiddle;
	SendMiddle=SendData;
	LE_0;
	CLK_0;
	for(i=0;i<16;i++)																	//高位先写入
	{
		if((SendMiddle & 0x8000)==0x8000)
		{
			DATA_1;
		}
		else
		{
			DATA_0;
		}
		Delay_1us (2);
		CLK_1;																					//写入数据
		Delay_1us (2);
		CLK_0;
		SendMiddle= SendMiddle<<1;
	}
	LE_1;
	Delay_1us (2);
	LE_0;
}

//-----------------------------------------------------------------
//Send11Bit(uint SendData) 
//-----------------------------------------------------------------
//
// 函数功能: 分频高11位数据写入
// 入口参数: SendData
// 返回参数: 无 
// 全局变量: 无
// 调用模块: 
// 注意事项: 
//-----------------------------------------------------------------
void Send11Bit(uint SendData)   										
{   
  uchar i;   
  LE_0;       
	CLK_0;   
  for( i = 0; i < 11; i ++ )   
  {   
		if((SendData & 0x8000)==0x8000)
		{
			DATA_1;
		}
		else
		{
			DATA_0;
		}		
    Delay_1us (2);      
		CLK_1;   
    Delay_1us (2);        
		CLK_0;   
    SendData = SendData << 1;   
  }   
}   

//-----------------------------------------------------------------
//Send8Bit( uchar SendData )   
//-----------------------------------------------------------------
//
// 函数功能: 分频低8位数据写入
// 入口参数: SendData
// 返回参数: 无 
// 全局变量: 无
// 调用模块: 
// 注意事项: 
//-----------------------------------------------------------------	
void Send8Bit( uchar SendData )   													//写入低8位数据
{   
  uchar i;    
  for( i = 0; i < 8; i ++ )   
  {   
		if((SendData & 0x80)==0x80)
		{
			DATA_1;
		}
		else
		{
			DATA_0;
		}				
    Delay_1us (2);        
		CLK_1;   
    Delay_1us (2);        
		CLK_0;   
    SendData = SendData << 1;   
   }   
		LE_1;        
  	Delay_1us (2);        
	  LE_0;   
}   

//-----------------------------------------------------------------
//SendReferF( void ) 
//-----------------------------------------------------------------
//
// 函数功能: 基准分频数据写入
// 入口参数: 无
// 返回参数: 无 
// 全局变量: 无
// 调用模块: 
// 注意事项: 
//-----------------------------------------------------------------
void SendReferF( void )   
{  		//针对采用12M晶振
  Send16Bit( 0x8961 ); // 基准分频 默认10k时钟输出
}   
   
void FrequenceToSend( uint FrequenceD )   
{   
  uchar AD = 0;       
  uint  ND = 0;      
	uint MiddleF = 2000;   
   
          MiddleF = FrequenceD;   								//分频系数P为32
          ND = ( uint  ) ( MiddleF / 32 );    	 //分频值=32*ND+AD;		
          AD = ( uchar ) ( MiddleF % 32 );   
          ND = ND << 5;                
	
					AD = AD << 1;       										//最低位为控制位，
					AD = AD & 0xfe;													//控制位 置低;
          Send11Bit( ND );        								
					Send8Bit( AD );   
}   


//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------

