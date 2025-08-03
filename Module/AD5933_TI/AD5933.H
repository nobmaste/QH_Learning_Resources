#ifndef _AD5933_H_
#define _AD5933_H_

#include "system_TM4C123.h"
#include "interface.H"


#define uint unsigned int



#define SDA_OUT() GPIOPinTypeGPIOOutput(AD5933_SDA_PORT, AD5933_SDA_PIN)
#define SDA_IN()  GPIOPinTypeGPIOInput(AD5933_SDA_PORT, AD5933_SDA_PIN)


//IO操作函数	 
#define AD5933_SCL_PORT GPIO_PORTE_BASE
#define AD5933_SDA_PORT GPIO_PORTE_BASE
#define AD5933_SCL_PIN  GPIO_PIN_5
#define AD5933_SDA_PIN  GPIO_PIN_4

		//// SCL 引脚控制
		//#define SCL_1() GPIOPinWrite(AD5933_SCL_PORT, AD5933_SCL_PIN, AD5933_SCL_PIN)
		//#define SCL_0() GPIOPinWrite(AD5933_SCL_PORT, AD5933_SCL_PIN, 0)

		//// SDA 引脚控制（输出时）
		//#define SDA_1() GPIOPinWrite(AD5933_SDA_PORT, AD5933_SDA_PIN, AD5933_SDA_PIN)
		//#define SDA_0() GPIOPinWrite(AD5933_SDA_PORT, AD5933_SDA_PIN, 0)

// SDA 引脚读入（输入时）
#define READ_SDA() (GPIOPinRead(AD5933_SDA_PORT, AD5933_SDA_PIN) ? 1 : 0)



#define AD5933_SYS_Init					(1)<<12
#define AD5933_Begin_Fre_Scan		(2)<<12
#define AD5933_Fre_UP						(3)<<12
#define AD5933_Fre_Rep					(4)<<12
#define AD5933_Get_Temp					(9)<<12
#define AD5933_Sleep						(10)<<12
#define AD5933_Standby					(11)<<12

#define AD5933_OUTPUT_2V				(0)<<9
#define AD5933_OUTPUT_1V				(3)<<9
#define AD5933_OUTPUT_400mV			(2)<<9
#define AD5933_OUTPUT_200mV			(1)<<9

#define AD5933_Gain_1						(1)<<8
#define AD5933_Gain_5						(0)<<8

#define AD5933_IN_MCLK					(0)<<3
#define AD5933_OUT_MCLK					(1)<<3

#define AD5933_Reset						(1)<<4

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;
typedef signed char    s8;
typedef signed short   s16;
typedef signed int     s32;



							
//定义函数
void Ini_I2c(void);
void SDA_1(void);
void SDA_0 (void);
void SCL_1(void);
void SCL_0 (void);
void GetACK(void);
void SendNACK(void);
void START(void);
void STOP(void);
void SendByte(u8 txd);	// 发送一个字节数据子函数
u8 ReadByte(void);  //读一个字节数据
void Write_Byte(char nAddr,uint nValue);
void SetPointer(char nAddr);
int Rece_Byte(char nAddr);
void Delay_ms(unsigned long nValue);
float Scale_imp (u8 *SValue,u8 *IValue,u8 *NValue,u8 *CValue);
u16 AD5933_Tempter(void);
float AD5933_Sweep (float Fre_Begin,float Fre_UP,u16 UP_Num,u16 OUTPUT_Vatage,u16 Gain,u16 SWeep_Rep);
float DA5933_Get_Cap(float *arads);
float DA5933_Get_L(float *arads);
float cot(float theta);
float DA5933_Get_RsRealImage(float *R_Impedance, float *realData, float *imageData,int k,float *arads);


#endif

