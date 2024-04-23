#include "AD9854.h"

u8 ad9854_freq[6];
u8 FreqWord[6];

AD9854::AD9854()
{
	delay_init(168);
	AD9854_IOinit();
	Func_Init();
}
//如果自己的程序里有鬼畜操作，
//如禁用引脚的JTAG/SW复用功能的操作
//可以通过析构函数将功能复原
AD9854::~AD9854()
{
	
}
//初始化AD9854的IO口
void AD9854::AD9854_IOinit()
{
	//初始化AD9854的地址线
	GPIO_InitTypeDef  Addr_InitStructure;
	RCC_AHB1PeriphClockCmd(AD9854_Addr_CLK, ENABLE);
	Addr_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	Addr_InitStructure.GPIO_OType = GPIO_OType_PP;
	Addr_InitStructure.GPIO_Pin = AD9854_Addr_Pin;
	Addr_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	Addr_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(AD9854_Addr_Port,&Addr_InitStructure);
	//初始化AD9854的控制引脚
	GPIO_InitTypeDef  Ctr_InitStructure;
	RCC_AHB1PeriphClockCmd(AD9854_Ctr_CLK, ENABLE);
	Ctr_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	Ctr_InitStructure.GPIO_OType = GPIO_OType_PP;
	Ctr_InitStructure.GPIO_Pin = AD9854_Ctr_Pin;
	Ctr_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	Ctr_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(AD9854_Ctr_Port,&Ctr_InitStructure);
}
//Master Reset：对DDS进行主复位，所有寄存器还原
void AD9854::reset()
{
	DDS_RESET = 0;
	DDS_RESET = 1;
	delay_ms(30);
	DDS_RESET = 0;
	
	DDS_WR = 1;
	DDS_RD = 1;
	UCLK = 1;
}
//向指定的寄存器中写入数据
void AD9854::write_data(u8 addr,u8 data)
{
	//读写禁止
	DDS_WR = 1;
	DDS_RD = 1;
	//写入地址
	GPIO_Write(GPIOD,addr);
	//写入数据
	Data_Out(data);
	//写使能
	DDS_WR = 0;
	//写失能
	DDS_WR = 1;	
}
//读取指定寄存器的数据
u8 AD9854::read_data(u8 addr)
{
	u8 data;
	//再次读取，读写失能
	DDS_WR = 1;
	DDS_RD = 1;
	//写入地址
	GPIO_Write(GPIOD,addr);
	//读使能
	DDS_RD = 0;
	//读取数据
	data = Data_In();
	//读失能
	DDS_RD = 1;	
	printf("\r\n address[0x%02x] = 0x%02x; \r\n",addr,data);
	return data;
}
//控制寄存器初始化[0x1d : 0x20]
void AD9854::Ctr_Reg_Init(u8 addr_1d, u8 addr_1e, u8 addr_1f, u8 addr_20)
{
	write_data(0x1d,addr_1d);
	write_data(0x1e,addr_1e);
	write_data(0x1f,addr_1f);
	write_data(0x20,addr_20);	
	Reg_Update();
}
//频率控制字1初始化
void AD9854::FTW1_Init(long Freq)
{
	Freq_convert(Freq);
	write_data(0x04,FreqWord[5]);
	write_data(0x05,FreqWord[4]);
	write_data(0x06,FreqWord[3]);
	write_data(0x07,FreqWord[2]);
	write_data(0x08,FreqWord[1]);
	write_data(0x09,FreqWord[0]);	
	Reg_Update();
}
//幅度控制字I/Q初始化
void AD9854::AM_Init(u8 iam_h, u8 iam_l, u8 qam_h, u8 qam_l)
{
	write_data(0x21,iam_h);
	write_data(0x22,iam_l);
	write_data(0x23,qam_h);
	write_data(0x24,qam_l);	
	Reg_Update();
}
//刷新寄存器数据，在使能了Int Update Clock位后不需要用
void AD9854::Reg_Update()
{
	UCLK = 0;
	delay_ms(1);
	UCLK = 1;
}
//芯片总体初始化
void AD9854::Func_Init()
{
	reset();
	Ctr_Reg_Init();
	FTW1_Init();
	AM_Init();	
	Reg_Update();
}
//频率转换函数，将48位FTW1频率控制字拆成6份
void AD9854::Freq_convert(long Freq)   
{
    ulong FreqBuf;
    ulong Temp=Freq_mult_ulong;   	       
	//将输入频率因子分为四个字节
	uchar Array_Freq[4];			     
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

void AD9854::Freq_Reset(long Freq)
{
	Freq_convert(Freq);
	write_data(0x04,FreqWord[5]);
	write_data(0x05,FreqWord[4]);
	write_data(0x06,FreqWord[3]);
	write_data(0x07,FreqWord[2]);
	write_data(0x08,FreqWord[1]);
	write_data(0x09,FreqWord[0]);

	Reg_Update();
}
//扫频请求,默认从1KHz开始，到4KHz结束，步进1Hz，间隔时间5000us,扫5个周期
void AD9854::Freq_Scan(long freq_low, long freq_high, long  steps, u32 intervals, u32 circles)
{
//	u8 flag = 0;
	u32 count = 0;
	long frequence = freq_low;
	Freq_Reset(frequence);
	
	while(1)
	{		
		Freq_Reset(frequence);		
//		if(flag == 0)
//		{
		frequence += steps;
//		}
//		if(flag == 1)
//		{
//			frequence -= steps;
//		}
		if(frequence == freq_high | frequence == freq_low)
		{
//			flag = !flag;
			count += 1;
//			if(count == circles)
//			{
//				Freq_Reset(frequence);	
//				break;
//			}
			frequence = freq_low;
		}
		delay_ms(intervals);	
	}	
}
/*************************************************************私有成员函数*************************************************************/
void AD9854::Data_Out(u8 data)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	GPIOE->ODR &= ~(0XFF<<8);		//只清除PE[8:15]
	GPIOE->ODR |= data<<8;			//PE[8:15]挂载数据

} 

u8 AD9854::Data_In(void)
{ 
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
	u8 dat;
	dat=(GPIOE->IDR)>>8;
	return dat;
}
