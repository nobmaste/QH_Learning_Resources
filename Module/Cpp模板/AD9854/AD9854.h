/*
 **********************************************************************
 *Copyright(c)		Lance Zhang
 *Identification	CHIC
 *Describtion		Template
 *
 *Present Version	1.0
 *Writer			Lance Zhang
 *Date				7/15/2019
 **********************************************************************
**/
//**********************以下为系统时钟以及其相关变量设置**************************
/* 
      此处根据自己的需要设置系统时钟以及与其相关的因子，一次需且只需开启一个   
      CLK_Set为时钟倍频设置，可设置4~20倍倍频，但最大不能超过300MHZ
      Freq_mult_ulong和Freq_mult_doulle均为2的48次方除以系统时钟，一个为长整形，一个为双精度型
	  
							@以下常量是在晶振为20MHz时求出的@
*/
#define ulong 	unsigned long 

/*
#define      CLK_Set            4
const ulong  Freq_mult_ulong  = 3518437;
const double Freq_mult_doulle = 3518437.2088832;
*/

/*
#define      CLK_Set            5
const ulong  Freq_mult_ulong  = 2814750;
const double Freq_mult_doulle = 2814749.76710656;
*/

 /*
#define      CLK_Set            6
const ulong  Freq_mult_ulong  = 2345625;
const double Freq_mult_doulle = 2345624.80592213;
  */

/*
#define      CLK_Set            7
const ulong  Freq_mult_ulong  = 2010536;
const double Freq_mult_doulle = 2010535.54793326;
*/

/*
#define      CLK_Set            8
const ulong  Freq_mult_ulong  = 1759219;
const double Freq_mult_doulle = 1759218.6044416;
*/

/*
#define      CLK_Set            9
const ulong  Freq_mult_ulong  = 1563750;
const double Freq_mult_doulle = 1563749.87061476;
*/

/*
#define      CLK_Set            10
const ulong  Freq_mult_ulong  = 1407375;
const double Freq_mult_doulle = 1407374.88355328;
*/
/*
#define      CLK_Set            11
const ulong  Freq_mult_ulong  = 1279432;
const double Freq_mult_doulle = 1279431.712321164;
*/

/*
#define      CLK_Set            12
const ulong  Freq_mult_ulong  = 1172812;
const double Freq_mult_doulle = 1172812.402961067;
*/

/*
#define      CLK_Set            13
const ulong  Freq_mult_ulong  = 1082596;
const double Freq_mult_doulle = 1082596.064271754;
*/

/*
#define      CLK_Set            14
const ulong  Freq_mult_ulong  = 1005268;
const double Freq_mult_doulle = 1005267.773966629;
*/


#define      CLK_Set            15
const ulong  Freq_mult_ulong  = 938250;
const double Freq_mult_doulle = 938249.9223688533;


#ifndef __AD9854_H
#define __AD9854_H
extern "C"
{
	#include "stm32f4xx_gpio.h"
	#include "sys.h"
	#include "delay.h"
	#include "usart.h"
}

//设置PB为输出
#define uint unsigned int
#define ulong unsigned long	
#define uchar unsigned char
//控制引脚命名
#define DDS_RESET 	PCout(6)	//复位
#define DDS_WR 		PCout(7)	//写入
#define DDS_RD 		PCout(8)	//读取
#define UCLK 		PCout(9)	//片选
//初始化端口设置
#define AD9854_Data_Port	GPIOE
#define AD9854_Addr_Port	GPIOD
#define AD9854_Ctr_Port		GPIOC
//时钟端口
#define AD9854_Ctr_CLK		RCC_AHB1Periph_GPIOC
#define AD9854_Addr_CLK		RCC_AHB1Periph_GPIOD
#define AD9854_Data_CLK		RCC_AHB1Periph_GPIOE
//数据引脚
#define AD9854_Data_Pin		GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | \
							GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15
//地址引脚
#define AD9854_Addr_Pin		GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 |	\
							GPIO_Pin_4 | GPIO_Pin_5
//控制引脚
#define AD9854_Ctr_Pin		GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9
//模式设置
#define Stone 	0x00
#define RFsk 	0x24

class	AD9854
{
//构造和析构函数
public:
	AD9854();
	~AD9854();

//用户常用的函数
public:
	//读取指定寄存器中的数据
	u8 read_data(u8 addr);
	//向指定寄存器中写入数据
	void write_data(u8 addr,u8 data);
	//MASTER RESET，复位所有寄存器
	void reset();
	//改变输出波的频率至设定的Freq
	void Freq_Reset(long Freq);
	//用于更新寄存器信息
	void Reg_Update();
	//扫频请求,默认从1KHz开始，到4KHz结束，步进1Hz，间隔时间5000us,扫5个周期
	void Freq_Scan(long freq_low = 1000, long freq_high = 4000, long  steps = 1, u32 intervals = 5000, u32 circles = 5);
//不常用函数，供用户调试用
public:	
	//控制寄存器初始化[0x1d : 0x20]
	//[0x1d]关闭比较器
	//[0x1e]有默认初始状态10倍频
	//[0x20]设置为可调节幅度，取消插值补偿
	void Ctr_Reg_Init(u8 addr_1d = 0x00, u8 addr_1e = CLK_Set, 
						u8 addr_1f = 0x00, u8 addr_20 = 0x60);
	//频率控制字1初始化,有默认值：1000Hz
	void FTW1_Init(long Freq = 1000);
	//幅度控制字I/Q初始化
	void AM_Init(u8 iam_h = 0xef, u8 iam_l = 0xff,
					u8 qam_h = 0xef, u8 qam_l = 0xff);
	//芯片总体初始化
	void Func_Init();

private:
	//初始化AD9854的IO口
	void AD9854_IOinit();
	//数据按位输入
	u8 Data_In(void);
	//数据按位输出
	void Data_Out(u8 data);
	//频率转换函数，将48位FTW1频率控制字拆成6份
	void Freq_convert(long Freq);
};

#endif
