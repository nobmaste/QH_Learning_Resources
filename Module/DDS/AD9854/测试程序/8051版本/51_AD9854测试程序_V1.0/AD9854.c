#include <AD9854.h>
#include <intrins.h>
#include <reg52.h>
#include <init.h>
//**********************以下为系统时钟以及其相关变量设置**************************/
/* 
  此处根据自己的需要设置系统时钟以及与其相关的因子，一次需且只需开启一个   
  CLK_Set为时钟倍频设置，可设置4~20倍倍频，但最大不能超过300MHZ
  Freq_mult_ulong和Freq_mult_doulle均为2的48次方除以系统时钟，一个为长整形，一个为双精度型
*/
// #define CLK_Set 7
// const ulong  Freq_mult_ulong  = 1340357;
// const double Freq_mult_doulle = 1340357.032;

//#define CLK_Set 9
//const ulong  Freq_mult_ulong  = 1042500;		 
//const double Freq_mult_doulle = 1042499.9137431;

// #define CLK_Set 8
// const ulong  Freq_mult_ulong  = 1172812;
// const double Freq_mult_doulle = 2932031.403;

#define CLK_Set 10
const ulong  Freq_mult_ulong  = 938250;
const double Freq_mult_doulle = 938249.9224;

uchar FreqWord[6];              //6个字节频率控制字

//**************************修改硬件时要修改的部分********************************/
 #define AD9854_DataBus P3
 #define AD9854_AdrBus  P1
 sbit AD9854_RST   = P2^0;    		//控制信号引脚
 sbit AD9854_SP    = P2^1;
 sbit AD9854_OSK   = P2^2; 			//osk
 sbit AD9854_UDCLK = P2^4; 
 sbit AD9854_WR    = P1^6;
 sbit AD9854_FDATA = P2^6;       //fsk/bpsk/hold
 sbit AD9854_RD    = P2^7;
  		


//====================================================================================
//函数名称:void AD9854_WR_Byte(uchar addr,uchar dat)
//函数功能:AD9854并行口写入数据
//入口参数:addr     6位地址
//         dat      写入的数据
//出口参数:无
//====================================================================================
void AD9854_WR_Byte(uchar addr,uchar dat)
{
	AD9854_AdrBus  = (addr&0x3f) | (P1&0xc0);
	AD9854_DataBus = dat;
	AD9854_WR      = 0;
	AD9854_WR      = 1;	
}

//====================================================================================
//函数名称:void AD9854_Init(void)
//函数功能:AD9854初始化
//入口参数:无
//出口参数:无
//====================================================================================
void AD9854_Init(void)
{
	AD9854_WR=1;//将读、写控制端口设为无效
	AD9854_RD=1;
	AD9854_UDCLK=0;
	AD9854_RST=1;                 //复位AD9854
	DelayMs(10);
	AD9854_RST=0;
  AD9854_SP = 1;
	
	//AD9854_WR_Byte(0x1d,0x10);	           //关闭比较器
	AD9854_WR_Byte(0x1d,0x00);	           //开启比较器
	AD9854_WR_Byte(0x1e,CLK_Set);	   //设置系统时钟倍频            
	AD9854_WR_Byte(0x1f,0x00);	   //设置系统为模式0，由外部更新
	AD9854_WR_Byte(0x20,0x60);	   //设置为可调节幅度，取消插值补偿

	AD9854_UDCLK=1;               //更新AD9854输出
  AD9854_UDCLK=0;
}

//====================================================================================
//函数名称:void Freq_convert(long Freq)
//函数功能:正弦信号频率数据转换
//入口参数:Freq   需要转换的频率，取值从0~SYSCLK/2
//出口参数:无   但是影响全局变量FreqWord[6]的值
//说明：   该算法位多字节相乘算法，有公式FTW = (Desired Output Frequency × 2N)/SYSCLK
//         得到该算法，其中N=48，Desired Output Frequency 为所需要的频率，即Freq，SYSCLK
//         为可编程的系统时钟，FTW为48Bit的频率控制字，即FreqWord[6]
//====================================================================================
void Freq_convert(long Freq)   
{
	ulong FreqBuf;
	ulong Temp=Freq_mult_ulong;   	       

	uchar Array_Freq[4];			     //将输入频率因子分为四个字节
	Array_Freq[0]=(uchar)Freq;
	Array_Freq[1]=(uchar)(Freq>>8);
	Array_Freq[2]=(uchar)(Freq>>16);
	Array_Freq[3]=(uchar)(Freq>>24);

	FreqBuf=Temp*Array_Freq[0];                  
	FreqWord[0]=FreqBuf;    
	FreqBuf>>=8;

	FreqBuf+=(Temp*Array_Freq[1]);
	FreqWord[1]=FreqBuf;
	FreqBuf>>=8;

	FreqBuf+=(Temp*Array_Freq[2]);
	FreqWord[2]=FreqBuf;
	FreqBuf>>=8;

	FreqBuf+=(Temp*Array_Freq[3]);
	FreqWord[3]=FreqBuf;
	FreqBuf>>=8;

	FreqWord[4]=FreqBuf;
	FreqWord[5]=FreqBuf>>8;			
}  

//====================================================================================
//函数名称:void AD9854_SetSine(ulong Freq,uint Shape)
//函数功能:AD9854正弦波产生程序
//入口参数:Freq   频率设置，取值范围为0~(1/2)*SYSCLK
//         Shape  幅度设置. 为12 Bit,取值范围为(0~4095) ,取值越大,幅度越大 
//出口参数:无
//====================================================================================
void AD9854_SetSine(ulong Freq,uint Shape)
{
	uchar count;
	uchar Adress;

	Adress = 0x04;                        //选择频率控制字地址的初值

	Freq_convert(Freq);		           //频率转换

	for(count=6;count>0;)	          //写入6字节的频率控制字  
  {
		AD9854_WR_Byte(Adress++,FreqWord[--count]);
  }
	
	AD9854_WR_Byte(0x21,Shape>>8);	  //设置I通道幅度
	AD9854_WR_Byte(0x22,(uchar)(Shape&0xff));
	
	AD9854_WR_Byte(0x23,Shape>>8);	  //设置Q通道幅度
	AD9854_WR_Byte(0x24,(uchar)(Shape&0xff));

	AD9854_UDCLK=1;                    //更新AD9854输出
  AD9854_UDCLK=0;
}