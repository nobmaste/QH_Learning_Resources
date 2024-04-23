//-----------------------------------------------------------------
// 程序描述:
// 　　stm32_AD9854测试程序_V2.1
// 作　　者: 凌智电子
// 开始日期: 2015-07-16
// 完成日期: 2015-07-16
// 修改日期: 2019-05-10
// 当前版本: V2.1
// 历史版本:
// 　- V1.0: 基本的AD9854并行控制输出，液晶显示功能
// 　- V1.1：并行控制转串行控制
// 　- V2.1: 通信方式：并行控制，LCD液晶显示提示；输出波形：正弦波、各个调制波形
// 调试工具: 凌智STM32F103核心板V2.2、1602液晶屏、LZE_ST_LINK2
// 说　　明: 
//				(1) 调试使用的系统时钟频率Fsysclk=72MHz;
//     		(2) AD9854的数据传输方式:并行传输方式;
//				(3) AD9854_IO与凌智STM32F103核心板V2.2连接方式如下:
//-----------------------------------------------------------------
//               IO 连接说明
//-----------------------------------------------------------------
//					 AD9854模块标号     |    STM32核心板   
//           PIV							 -->	 +5V
//           GND							 -->	 GND
//					 A[5:0]   				 -->   PA[5:0](GPIOA[5:0])
//					 D[7:0]    			   -->   PB[15:8](GPIOB[15:8])
//					 RD                -->   PC8(GPIOC_PIN8)
//					 WR                -->   PC9(GPIOC_PIN9)
//					 UD(UDCLK)         -->   PC10(GPIOC_PIN10)
//					 RT(RESET)         -->   PC11(GPIOC_PIN11)
//					 SP                -->   PC12(GPIOC_PIN12)
//
//					 AD9854模块标号     |    外部信号比如信号源连接控制         
//					 OSK      					外部控制信号接在OSK引脚来控制
//					 FD(FDSTA)       		外部信号接AD9854中的FSK(S2)射频头来控制也可以直接输入到AD9854中的FD引脚
//-----------------------------------------------------------------
//        (4) 由于AD9854功耗高, 请保证AD9854模块的供电功率, 供电电源建议5V1A电源
//        (5) 本测试程序实现正弦波和各个调制波形输出功能, 4个输出通道均可输出波形
//				(6) 按键操作步骤: STM32核心板上按键选择控制输出波形类型
//						K1: 功能选择键, 按下K1分别依次顺序循环选择SINE、FSK、BPSK、RFSK、AM、OSK和CHIRP模式
//						K2: SINE模式下使用, 每按一下, 表示频率加 
//						K3: SINE模式下使用, 每按一下, 表示频率减
//						K4: 退出当前模式, 然后可以使用K1键继续选择需要的模式
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// 头文件包含
//-----------------------------------------------------------------
#include <stm32f10x.h>
#include "Delay.h"
#include "CharLCD.h"
#include "PeripheralInit.h"
#include "AD9854.h"
#include "key.h"
#include "tim3.h"

//-----------------------------------------------------------------
// AM波中调制波所用的正弦波数据
uint16_t Sinx_data[256] = {	
0x7FF,0x831,0x863,0x896,0x8C8,0x8FA,0x92B,0x95D,0x98E,0x9C0,0x9F1,0xA21,0xA51,0xA81,0xAB1,0xAE0,
0xB0F,0xB3D,0xB6A,0xB98,0xBC4,0xBF0,0xC1C,0xC46,0xC71,0xC9A,0xCC3,0xCEB,0xD12,0xD38,0xD5E,0xD83,
0xDA7,0xDCA,0xDEC,0xE0D,0xE2E,0xE4D,0xE6C,0xE89,0xEA5,0xEC1,0xEDB,0xEF5,0xF0D,0xF24,0xF3A,0xF4F,
0xF63,0xF75,0xF87,0xF97,0xFA6,0xFB4,0xFC1,0xFCD,0xFD7,0xFE0,0xFE8,0xFEF,0xFF5,0xFF9,0xFFC,0xFFE,
0xFFE,0xFFE,0xFFC,0xFF9,0xFF5,0xFEF,0xFE8,0xFE0,0xFD7,0xFCD,0xFC1,0xFB4,0xFA6,0xF97,0xF87,0xF75,
0xF63,0xF4F,0xF3A,0xF24,0xF0D,0xEF5,0xEDB,0xEC1,0xEA5,0xE89,0xE6C,0xE4D,0xE2E,0xE0D,0xDEC,0xDCA,
0xDA7,0xD83,0xD5E,0xD38,0xD12,0xCEB,0xCC3,0xC9A,0xC71,0xC46,0xC1C,0xBF0,0xBC4,0xB98,0xB6A,0xB3D,
0xB0F,0xAE0,0xAB1,0xA81,0xA51,0xA21,0x9F1,0x9C0,0x98E,0x95D,0x92B,0x8FA,0x8C8,0x896,0x863,0x831,
0x7FF,0x7CD,0x79B,0x768,0x736,0x704,0x6D3,0x6A1,0x670,0x63E,0x60D,0x5DD,0x5AD,0x57D,0x54D,0x51E,
0x4EF,0x4C1,0x494,0x466,0x43A,0x40E,0x3E2,0x3B8,0x38D,0x364,0x33B,0x313,0x2EC,0x2C6,0x2A0,0x27B,
0x257,0x234,0x212,0x1F1,0x1D0,0x1B1,0x192,0x175,0x159,0x13D,0x123,0x109,0x0F1,0x0DA,0x0C4,0x0AF,
0x09B,0x089,0x077,0x067,0x058,0x04A,0x03D,0x031,0x027,0x01E,0x016,0x00F,0x009,0x005,0x002,0x000,
0x000,0x000,0x002,0x005,0x009,0x00F,0x016,0x01E,0x027,0x031,0x03D,0x04A,0x058,0x067,0x077,0x089,
0x09B,0x0AF,0x0C4,0x0DA,0x0F1,0x10A,0x123,0x13D,0x159,0x175,0x192,0x1B1,0x1D0,0x1F1,0x212,0x234,
0x257,0x27B,0x2A0,0x2C6,0x2EC,0x313,0x33B,0x364,0x38D,0x3B8,0x3E2,0x40E,0x43A,0x466,0x494,0x4C1,
0x4EF,0x51E,0x54D,0x57D,0x5AD,0x5DD,0x60E,0x63E,0x670,0x6A1,0x6D3,0x704,0x736,0x768,0x79B,0x7CD
};
//-----------------------------------------------------------------

u8  Key_Count = 0,Key_Value = 0;
u32 F=1000;  		//设置正弦波的频率（1~100MHz）
u16 A=4095;			//设置正弦波的幅度（0~4095）
u8 numb;

//-----------------------------------------------------------------
// 主程序
//-----------------------------------------------------------------
int main(void)
{			
  Delay_1ms(100);
	PeripheralInit();
	GPIO_AD9854_Configuration();
	WrCLcdC(0x06);
	WriteString(1, 1,"  AD9854 TEST!  ");
	WriteString(2, 1,"K1:  Select Wave");	
//	LED1_ON;
	tim_set (10000, 72000000);
	while (1)
	{	
		Key_Value=Key_Scan();
		if(Key_Count!=1)
		{
		if(Key_Value==1)
		{
			Key_Count ++;
			WriteString(2, 1,"                 ");	
			switch (Key_Count)
  	  {
			 	case  1 :	{                             
  	  			        AD9854_Init();                       	
										WriteString(2, 1,"     SINE        ");						// 正弦SINE输出模式
  	              }break;
  	   case  3 :	{
  				          AD9854_InitFSK();
										AD9854_SetFSK(10000,100000);										 // AD9854_SetFSK(频率1,频率2)
										WriteString(2, 1,"     FSK         ");
  	              }break;
  	   case 4 :	{
  	  			        AD9854_InitBPSK();                      
                    AD9854_SetBPSK(0, 8192);    										 // AD9854_SetBPSK(相位1,相位2) 	
										WriteString(2, 1,"     BPSK        ");                  
                  }break;
  	   case  5 :	{
  				          AD9854_InitRFSK();                      
                    AD9854_SetRFSK(1000000,40000000,100000,524288);  //AD9854_SetRFSK(低频率，高频率，步进频率，斜率时钟控制)
										WriteString(2, 1,"     RFSK        ");    
                  }break;
  	   case  6 :	{
  	  			        AD9854_InitAM();
										am_test(100,100);		  				            			 //am_test(调制波频率，调制度设置)				
										WriteString(2, 1,"      A M        ");

                  }break;
  	   case  7 :	{
  				          AD9854_InitOSK();                     
                    AD9854_SetOSK (40);    												   // OSK高电平，幅值增加；低电平幅值减少，100为变化的斜率（满幅输出） 		
										WriteString(2, 1,"      OSK        ");     
                   }break;
		   case  8 :	{
  				          AD9854_InitChirp();                     
                    AD9854_SetChirp (100000,1000,5000);   				   // 啁啾模式,	AD9854_SetChirp (低频率,步进,斜率时钟)   	
										WriteString(2, 1,"     CHIRP        ");
										Key_Count= 0 ;
                   }break;
  	    default :	break;
		 }
	 }
		}	
		if(Key_Count == 1)
		{
			AD9854_SetSine (F, A);
			numb = Key_Scan();
			if (numb == 2)
			{
				if (F < 140000000)
				{
					if (F < 1000)									// 输出频率小于1kHz，按100Hz步进
					{
						F += 100;
					}
					else if (F< 10000)						// 输出频率小于10kHz，按1kHz步进
					{
						F += 1000;
					}
					else if (F < 100000)					// 输出频率小于100kHz，按10kHz步进
					{
						F += 10000;
					}
					else if (F < 1000000)					// 输出频率小于1MHz，按100kHz步进
					{
						F += 100000;
					}
					else if (F < 10000000)				// 输出频率小于10MHz，按1MHz步进
					{
						F += 1000000;
					}
					else 
					{
						F += 5000000;								// 输出频率小于140MHz，按5MHz步进
					}
				}
			}
			else if (numb == 3)
			{
				if (F > 100)
				{
					if (F > 10000000)
					{
						F -= 5000000;
					}
					else if (F > 1000000)
					{
						F -= 1000000;
					}
					else if (F > 100000)
					{
						F -= 100000;
					}
					else if (F > 10000)
					{
						F -= 10000;
					}
					else if (F > 1000)
					{
						F -= 1000;
					}
					else if (F > 100)
					{
						F -= 100;
					}
				}	
			}else if(numb==4)
			{
				Key_Count=2;
			}
		}
		if(Key_Count==6)
		{
				
			if (TIM_FLAG == 1)																			// 
				{
					TIM_FLAG = 0;
//					LED1_Toggle;
					add += step_up;		
					amp_buf = (uint16_t)((float)4095 * (0x0FFF - (((float)modu_buf * Sinx_data[add>>8]/100)))/ 4096);
			
					AD9854_SetAM(600000,amp_buf);
				}
		}
 }
}


//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
