/*--------------------------------------------------------------
头文件名：AD9854.h				  
文件描述：数据处理头文件
---------------------------------------------------------------*/
#ifndef _AD9854_H_
#define _AD9854_H_

#include "stm32f429_winner.h"
//----------------------------------------------------------------
// AD9854_I/O控制线
//-----------------------------------------------------------------
#define AD9854_RD_Set 		(HAL_GPIO_WritePin ( GPIOC, GPIO_PIN_8, GPIO_PIN_SET   ) )         
#define AD9854_RD_Clr 		(HAL_GPIO_WritePin ( GPIOC, GPIO_PIN_8, GPIO_PIN_RESET ) )

#define AD9854_WR_Set 		(HAL_GPIO_WritePin ( GPIOC, GPIO_PIN_9, GPIO_PIN_SET   ) )       
#define AD9854_WR_Clr 		(HAL_GPIO_WritePin ( GPIOC, GPIO_PIN_9, GPIO_PIN_RESET ) )

#define AD9854_UDCLK_Set  (HAL_GPIO_WritePin ( GPIOC, GPIO_PIN_10, GPIO_PIN_SET   ) ) 
#define AD9854_UDCLK_Clr  (HAL_GPIO_WritePin ( GPIOC, GPIO_PIN_10, GPIO_PIN_RESET ) )

#define AD9854_RST_Set 	  (HAL_GPIO_WritePin ( GPIOC, GPIO_PIN_11, GPIO_PIN_SET   ) )     //开关电源板
#define AD9854_RST_Clr    (HAL_GPIO_WritePin ( GPIOC, GPIO_PIN_11, GPIO_PIN_RESET ) )

#define AD9854_SP_Set     (HAL_GPIO_WritePin ( GPIOC, GPIO_PIN_12, GPIO_PIN_SET   ) )
#define AD9854_SP_Clr     (HAL_GPIO_WritePin ( GPIOC, GPIO_PIN_12, GPIO_PIN_RESET ) )

#define AD9854_OSK_Set    (HAL_GPIO_WritePin ( GPIOC, GPIO_PIN_13, GPIO_PIN_SET   ) ) 
#define AD9854_OSK_Clr    (HAL_GPIO_WritePin ( GPIOC, GPIO_PIN_13, GPIO_PIN_RESET ) )

//#define AD9854_FDATA_Set  (HAL_GPIO_WritePin ( GPIOB, GPIO_PIN_3, GPIO_PIN_SET   ) )			   //fsk/bpsk/hold  
//#define AD9854_FDATA_Clr  (HAL_GPIO_WritePin ( GPIOB, GPIO_PIN_3, GPIO_PIN_RESET ) )
// I/O驱动  使用别名
#define SPI_IO_RST_Set (HAL_GPIO_WritePin ( GPIOB, GPIO_PIN_0, GPIO_PIN_SET   ) )
#define SPI_IO_RST_Clr (HAL_GPIO_WritePin ( GPIOB, GPIO_PIN_0, GPIO_PIN_RESET   ) )
#define SPI_SDO_Set    (HAL_GPIO_WritePin ( GPIOB, GPIO_PIN_1, GPIO_PIN_SET   ) )
#define SPI_SDO_Clr    (HAL_GPIO_WritePin ( GPIOB, GPIO_PIN_1, GPIO_PIN_RESET   ) )
#define SPI_SDI_Set 	 (HAL_GPIO_WritePin ( GPIOB, GPIO_PIN_2, GPIO_PIN_SET   ) )
#define SPI_SDI_Clr 	 (HAL_GPIO_WritePin ( GPIOB, GPIO_PIN_2, GPIO_PIN_RESET   ) )

 // AD9854寄存器地址
#define PHASE1	0x00	  //phase adjust register #1
#define PHASE2  0x01		//phase adjust register #2
#define FREQ1   0x02		//frequency tuning word 1
#define FREQ2   0x03		//frequency tuning word 2
#define DELFQ   0x04		//delta frequency word
#define UPDCK   0x05		//update clock
#define RAMPF   0x06		//ramp rate clock
#define CONTR   0x07		//control register
#define SHAPEI  0x08		//output shape key I mult
#define SHAPEQ  0x09		//output shape key Q mult 
#define RAMPO   0x0A		//output shape key ramp rate
#define CDAC    0x0B		//QDAC
extern uint16_t add;
extern uint16_t step_up;
extern uint16_t amp_buf;
extern uint8_t modu_buf;
//**************************以下部分为函数定义********************************
extern void GPIO_AD9854_Configuration(void);						// AD9854_IO口初始化
static void AD9854_WR_Byte(u8 Adata);	  
extern void AD9854_Init(void);						  
static void Freq_convert(long Freq);	         	  
extern void AD9854_SetSine(u32 Freq,u16 Shape);	  
static void Freq_double_convert(double Freq);		  
extern void AD9854_SetSine_double(double Freq,u16 Shape);
extern void AD9854_InitFSK(void);				
extern void AD9854_SetFSK(u32 Freq1,u32 Freq2);					  
extern void AD9854_InitBPSK(void);					  
extern void AD9854_SetBPSK(u16 Phase1,u16 Phase2);					
extern void AD9854_InitOSK(void);					 
extern void AD9854_SetOSK(u8 RateShape);					  
extern void AD9854_InitAM(void);					 
extern void AD9854_SetAM(u16 Shape);				
extern void AD9854_InitRFSK(void);					 
extern void AD9854_SetRFSK(u32 Freq_Low,u32 Freq_High,u32 Freq_Up_Down,u32 FreRate);				
extern void Sweep_out (void);
extern void AD9854_InitChirp(void);
extern void AD9854_SetChirp(unsigned long Freq_Low,unsigned long Freq_Up_Down,unsigned long FreRate);
extern void am_test (uint32_t fund_fre, uint8_t modulation);
#endif
