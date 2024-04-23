/*--------------------------------------------------------------
头文件名：AD9854.h				  
文件描述：数据处理头文件
---------------------------------------------------------------*/
#ifndef _AD9854_H_
#define _AD9854_H_
/*--------------------------------------------------------------*/
#include "type.h"

//**************************以下部分为函数定义********************************
extern void AD9854_WR_Byte(uchar Adata);	  
extern void AD9854_Init(void);						  
static void Freq_convert(long Freq);	         	  
extern void AD9854_SetSine(ulong Freq,uint Shape);	  
static void Freq_double_convert(double Freq);		  
extern void AD9854_SetSine_double(double Freq,uint Shape);
extern void AD9854_InitFSK(void);				
extern void AD9854_SetFSK(ulong Freq1,ulong Freq2);					  
extern void AD9854_InitBPSK(void);					  
extern void AD9854_SetBPSK(uint Phase1,uint Phase2);					
extern void AD9854_InitOSK(void);					 
extern void AD9854_SetOSK(uchar RateShape);					  
extern void AD9854_InitAM(void);					 
extern void AD9854_SetAM(uint Shape);					
extern void AD9854_InitRFSK(void);					 
extern void AD9854_SetRFSK(ulong Freq_Low,ulong Freq_High,ulong Freq_Up_Down,ulong FreRate);				

#endif