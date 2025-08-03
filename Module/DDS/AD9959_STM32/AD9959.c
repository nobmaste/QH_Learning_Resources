
/************************************************************
                    AD9959 驱动程序
					AD9959--单片机 
硬件连接:  CS			——PD11;     
          SCLK 		——PG5;   
          UPDATE	——PG10;	  
          SP0    	——PD15;   
          SP1			——PD0;     
          SP2			——PD1; 
          SP3			——PD5;     
          SDIO0		——PG2;     
          SDIO1		——PG4;     
          SDIO2		——PG3;     
          SDIO3		——PD12;     
   AD9959_PWR(PDC)——PD14;     
          RST			——PE9;         
          GND			--GND(0V)  
//AD9959.c
//康威电子工作室
//说明：本程序基于硬件的外接晶振为25MHZ
**************************************************************/

#include "AD9959.h"
															
uint8_t FR1_DATA[3] = {0xD0,0x00,0x00};//VCO gain control[23]=1系统时钟高于255Mhz; PLL[22:18]=10100,20倍频,20*25M=500MHZ; Charge pump control = 75uA 


uint8_t FR2_DATA[2] = {0x00,0x00};	// 双方向扫描，即从起始值扫到结束值后，又从结束值扫到起始值
//uint8_t FR2_DATA[2] = {0x80,0x00};// 单方向扫描，即从起始值扫到结束值后，又从起始值扫到结束值，以此类推


double ACC_FRE_FACTOR = 8.589934592;	//频率因子8.589934592=(2^32)/500000000 其中500M=25M*20(倍频数可编程)

uint8_t CFR_DATA[3] = {0x00,0x03,0x02};//default Value = 0x000302	   //通道功能寄存器

																	
																
																	
/************************************************************
** 函数名称 ：void AD9959_Init(void)  
** 函数功能 ：初始化控制AD9959需要用到的IO口,及寄存器
** 入口参数 ：无
** 出口参数 ：无
** 函数说明 ：无
**************************************************************/
void AD9959_Init(void)  
{ 
//	GPIO_InitTypeDef  GPIO_InitStructure;       
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE);	 //PA,PB,PC端口时钟使能
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_2|GPIO_Pin_7|GPIO_Pin_6|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;//初始化管脚PA2.3.4.5.6.7.8.9.10
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
//	GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_10;//初始化管脚PB0.1.10			
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		 //IO口速度为2MHz
//	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB
//	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	//初始化管脚PC0			 
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;		 //IO口速度为2MHz
//	GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOC
		
	Intserve();  //IO口电平状态初始化
  IntReset();  //AD9959复位  
	
	//初始化功能寄存器
  AD9959_WriteData(FR1_ADD,3,FR1_DATA);//写功能寄存器1
  AD9959_WriteData(FR2_ADD,2,FR2_DATA);//
} 

//延时
void delay1 (uint32_t length)
{
	length = length*12;
   while(length--);
}
/************************************************************
** 函数名称 ：void Intserve(void)		   
** 函数功能 ：IO口电平状态初始化
**************************************************************/
void Intserve(void)		   
{   
		AD9959_PWR_LOW();
		CS_HIGH();          // 原 CS = 1
		SCLK_LOW();         // 原 SCLK = 0
		UPDATE_LOW();

		PS0_LOW();
		PS1_LOW();
		PS2_LOW();
		PS3_LOW();

		SDIO0_LOW();
		SDIO1_LOW();
		SDIO2_LOW();
		SDIO3_LOW();
}

/************************************************************
** 函数名称 ：void IntReset(void)		   
** 函数功能 ：AD9959复位
**************************************************************/
void IntReset(void)	  
{
	RESET_LOW();    // 代替 Reset = 0;
	delay1(1);
	RESET_HIGH();   // 代替 Reset = 1;
	delay1(30);
	RESET_LOW();    // 代替 Reset = 0;
}

/************************************************************
** 函数名称 void IO_Update(void)  
** 函数功能 ： AD9959更新数据
**************************************************************/
void IO_Update(void)  
{
	UPDATE_LOW();
	delay1(2);
	UPDATE_HIGH();
	delay1(4);
	UPDATE_LOW();
}

/************************************************************
** 函数名称 ：void AD9959_WriteData(u8 RegisterAddress, u8 NumberofRegisters, u8 *RegisterData)
** 函数功能 ：使用模拟SPI向AD9959写数据
** 入口参数 ：RegisterAddress: 寄存器地址
						NumberofRegisters: 要写入的字节数
						*RegisterData: 数据起始地址
** 出口参数 ：无
** 函数说明 ：无
**************************************************************/
void AD9959_WriteData(uint8_t RegisterAddress, uint8_t NumberofRegisters, uint8_t *RegisterData)
{
	uint8_t	ControlValue = 0;
	uint8_t	ValueToWrite = 0;
	uint8_t	RegisterIndex = 0;
	uint8_t	i = 0;

	ControlValue = RegisterAddress;
//写入地址
	SCLK_LOW();
	CS_LOW();
	for(i=0; i<8; i++)
	{
		SCLK_LOW();
		if(0x80 == (ControlValue & 0x80))
			SDIO0_HIGH();	  	
		else
			SDIO0_LOW(); 
		SCLK_HIGH();
		ControlValue <<= 1;
	}
	SCLK_LOW();;
//写入数据
	for (RegisterIndex=0; RegisterIndex<NumberofRegisters; RegisterIndex++)
	{
		ValueToWrite = RegisterData[RegisterIndex];
		for (i=0; i<8; i++)
		{
			SCLK_LOW();;
			if(0x80 == (ValueToWrite & 0x80))
				SDIO0_HIGH();	  
			else
				SDIO0_LOW();
			SCLK_HIGH();	
			ValueToWrite <<= 1;
		}
		SCLK_LOW();;		
	}	
  CS_HIGH();

} 

/************************************************************
** 函数名称 ：void Write_CFTW0(uint32_t fre)
** 函数功能 ：写CFTW0通道频率转换字寄存器
** 入口参数 ： Freq:	写入频率，范围0~200 000 000 Hz
** 出口参数 ：无
** 函数说明 ：无
**************************************************************/
void Write_CFTW0(uint32_t fre)
{
		uint8_t CFTW0_DATA[4] ={0x00,0x00,0x00,0x00};	//中间变量
	  uint32_t Temp;            
	  Temp=(uint32_t)fre*ACC_FRE_FACTOR;	 
	  CFTW0_DATA[3]=(uint8_t)Temp;
	  CFTW0_DATA[2]=(uint8_t)(Temp>>8);
	  CFTW0_DATA[1]=(uint8_t)(Temp>>16);
	  CFTW0_DATA[0]=(uint8_t)(Temp>>24);		
		AD9959_WriteData(CFTW0_ADD,4,CFTW0_DATA);//CTW0 address 0x04
}

/************************************************************
** 函数名称 ：void Write_ACR(uint16_t Ampli)
** 函数功能 ：写ACR通道幅度转换字寄存器
** 入口参数 ：Ampli:    输出幅度,范围0~1023，控制值0~1023对应输出幅度0~500mVpp左右
** 出口参数 ：无
** 函数说明 ：无
**************************************************************/
void Write_ACR(uint16_t Ampli)
{ 
	uint32_t A_temp=0;
	uint8_t ACR_DATA[3] = {0x00,0x00,0x00};//default Value = 0x--0000 Rest = 18.91/Iout 
  A_temp=Ampli|0x1000;
	
	ACR_DATA[1] = (uint8_t)(A_temp>>8); //高位数据
	ACR_DATA[2] = (uint8_t)A_temp;  //低位数据
  AD9959_WriteData(ACR_ADD,3,ACR_DATA); //ACR address 0x06.CHn设定幅度
}

/************************************************************
** 函数名称 ：void Write_CPOW0(uint16_t Phase)
** 函数功能 ：写CPOW0通道相位转换字寄存器
** 入口参数 ：Phase:		输出相位,范围：0~16383(对应角度：0°~360°)
** 出口参数 ：无
** 函数说明 ：无
**************************************************************/
void Write_CPOW0(uint16_t Phase)
{
	uint8_t CPOW0_data[2] = {0x00,0x00};
	CPOW0_data[1]=(uint8_t)Phase;
	CPOW0_data[0]=(uint8_t)(Phase>>8);

	AD9959_WriteData(CPOW0_ADD,2,CPOW0_data);//CPOW0 address 0x05.CHn设定相位
}

/************************************************************
** 函数名称 ：void Write_LSRR(uint8_t rsrr,uint8_t fsrr)
** 函数功能 ：写LSRR线性扫描斜率寄存器
** 入口参数 ：	rsrr:	上升斜率,范围：0~255
							fsrr:	下降斜率,范围：0~255
** 出口参数 ：无
** 函数说明 ：无
**************************************************************/
void Write_LSRR(uint8_t rsrr,uint8_t fsrr)
{
	uint8_t LSRR_data[2]={0x00,0x00};

	LSRR_data[1]=rsrr;	
	LSRR_data[0]=fsrr;//高8位下降斜率

	AD9959_WriteData(LSRR_ADD,2,LSRR_data);
}

/************************************************************
** 函数名称 ：void Write_RDW(uint32_t r_delta)
** 函数功能 ：写RDW上升增量寄存器
** 入口参数 ：r_delta:上升增量,0-4294967295
** 出口参数 ：无
** 函数说明 ：无
**************************************************************/
void Write_RDW(uint32_t r_delta)
{
		uint8_t RDW_data[4] ={0x00,0x00,0x00,0x00};	//中间变量          
 
	  RDW_data[3]=(uint8_t)r_delta;
	  RDW_data[2]=(uint8_t)(r_delta>>8);
	  RDW_data[1]=(uint8_t)(r_delta>>16);
	  RDW_data[0]=(uint8_t)(r_delta>>24);		
		AD9959_WriteData(RDW_ADD,4,RDW_data);
}

/************************************************************
** 函数名称 ：void Write_FDW(uint32_t f_delta)
** 函数功能 ：写FDW下降增量寄存器
** 入口参数 ：f_delta:下降增量,0-4294967295
** 出口参数 ：无
** 函数说明 ：无
**************************************************************/
void Write_FDW(uint32_t f_delta)
{
		uint8_t FDW_data[4] ={0x00,0x00,0x00,0x00};	//中间变量          
 
	  FDW_data[3]=(uint8_t)f_delta;
	  FDW_data[2]=(uint8_t)(f_delta>>8);
	  FDW_data[1]=(uint8_t)(f_delta>>16);
	  FDW_data[0]=(uint8_t)(f_delta>>24);		
		AD9959_WriteData(FDW_ADD,4,FDW_data);
}

/************************************************************
** 函数名称 ：void Write_Profile_Fre(uint8_t profile,uint32_t data)
** 函数功能 ：写Profile寄存器
** 入口参数 ：profile:	profile号(0~14)
							data:	写入频率，范围0~200 000 000 Hz
** 出口参数 ：无
** 函数说明 ：无
**************************************************************/
void Write_Profile_Fre(uint8_t profile,uint32_t data)
{
		uint8_t profileAddr;
		uint8_t Profile_data[4] ={0x00,0x00,0x00,0x00};	//中间变量
	  uint32_t Temp;            
	  Temp=(uint32_t)data*ACC_FRE_FACTOR;	   //将输入频率因子分为四个字节  4.294967296=(2^32)/500000000
	  Profile_data[3]=(uint8_t)Temp;
	  Profile_data[2]=(uint8_t)(Temp>>8);
	  Profile_data[1]=(uint8_t)(Temp>>16);
	  Profile_data[0]=(uint8_t)(Temp>>24);
		profileAddr = PROFILE_ADDR_BASE + profile;
		
		AD9959_WriteData(profileAddr,4,Profile_data);
}
/************************************************************
** 函数名称 ：void Write_Profile_Ampli(uint8_t profile,uint16_t data)
** 函数功能 ：写Profile寄存器
** 入口参数 ：profile:	profile号(0~14)
							data:	 写入幅度,范围0~1023，
** 出口参数 ：无
** 函数说明 ：无
**************************************************************/
void Write_Profile_Ampli(uint8_t profile,uint16_t data)
{
		uint8_t profileAddr;
		uint8_t Profile_data[4] ={0x00,0x00,0x00,0x00};	//中间变量

		//为幅度调制时幅度数据为[31:22]位
	  Profile_data[1]=(uint8_t)(data<<6);//[23:22]
	  Profile_data[0]=(uint8_t)(data>>2);//[31:24]
		
		profileAddr = PROFILE_ADDR_BASE + profile;
		
		AD9959_WriteData(profileAddr,4,Profile_data);//写入32位数据
}
/************************************************************
** 函数名称 ：void Write_Profile_Phase(uint8_t profile,uint16_t data)
** 函数功能 ：写Profile寄存器
** 入口参数 ：profile:	profile号(0~14)
							data:	 写入相位,范围：0~16383
** 出口参数 ：无
** 函数说明 ：无
**************************************************************/
void Write_Profile_Phase(uint8_t profile,uint16_t data)
{
		uint8_t profileAddr;
		uint8_t Profile_data[4] ={0x00,0x00,0x00,0x00};	//中间变量

		//为相位调制时相位数据为[31:18]位
	  Profile_data[1]=(uint8_t)(data<<2);//[23:18]
	  Profile_data[0]=(uint8_t)(data>>6);//[31:24]
		
		profileAddr = PROFILE_ADDR_BASE + profile;
		
		AD9959_WriteData(profileAddr,4,Profile_data);//写入32位数据
}



/************************************************************
** 函数名称 ：void AD9959_Set_Fre(uint8_t Channel,uint32_t Freq)
** 函数功能 ：设置通道的输出频率
** 入口参数 ：Channel:  输出通道  CH0-CH3
						 Freq:     输出频率，范围0~200 000 000 Hz
** 出口参数 ：无
** 函数说明 ：无
**************************************************************/
void AD9959_Set_Fre(uint8_t Channel,uint32_t Freq)
{	 
		uint8_t CHANNEL[1] = {0x00};
		
		CHANNEL[0]=Channel;
		AD9959_WriteData(CSR_ADD,1,CHANNEL);//控制寄存器写入CHn通道，选择CHn
    Write_CFTW0(Freq);//输出CHn设定频率																																			 
} 

/************************************************************
** 函数名称 ：void AD9959_Set_Amp(uint8_t Channel, uint16_t Ampli)
** 函数功能 ：设置通道的输出幅度
** 入口参数 ：Channel:  输出通道 CH0-CH3
							Ampli:    输出幅度,范围0~1023，控制值0~1023对应输出幅度0~500mVpp左右
** 出口参数 ：无
** 函数说明 ：无
**************************************************************/
void AD9959_Set_Amp(uint8_t Channel, uint16_t Ampli)
{ 
	uint8_t CHANNEL[1] = {0x00};

	CHANNEL[0]=Channel;
	AD9959_WriteData(CSR_ADD,1,CHANNEL); //控制寄存器写入CHn通道，选择CHn
	Write_ACR(Ampli);							//	CHn设定幅度
}

/************************************************************
** 函数名称 ：void AD9959_Set_Phase(uint8_t Channel,uint16_t Phase)
** 函数功能 ：设置通道的输出相位
** 入口参数 ：Channel:  输出通道 CH0-CH3
							Phase:		输出相位,范围：0~16383(对应角度：0°~360°)
** 出口参数 ：无
** 函数说明 ：无
**************************************************************/
void AD9959_Set_Phase(uint8_t Channel,uint16_t Phase)
{
	uint8_t CHANNEL[1] = {0x00};
	CHANNEL[0]=Channel;

	AD9959_WriteData(CSR_ADD,1,CHANNEL); //控制寄存器写入CHn通道，选择CHn
	Write_CPOW0(Phase);//CHn设定相位
}

/************************************************************
** 函数名称 ：void AD9959_Modulation_Init(uint8_t Channel,uint8_t Modulation,uint8_t Sweep_en,uint8_t Nlevel)
** 函数功能 ：设置各个通道的调制模式。
** 入口参数 ： Channel:  	输出通道 CH0-CH3
							Modulation:	调制模式DISABLE_Mod，ASK，FSK，PSK
							Sweep_en:		线性扫描模式 SWEEP_ENABLE启用、SWEEP_DISABLE不启用；启用时Nlevel只能是LEVEL_MOD_2
							Nlevel：		调制电平选择 LEVEL_MOD_2、4、8、16
** 出口参数 ：无
** 函数说明 ：如将调制电平设置为2电平调制时，对应的P0-P3引脚上的高低电平分别控制CH0-CH3通道(如果对应通道开启的话)
							如将调制电平设置为4电平调制时，对应的P0，P1和P2,P3引脚上的高低电平分别控制CH0-CH1通道(如果对应通道开启的话)
							由于AD9959只有P0-P3,4个用于调制控制的引脚，因此输出在4电平调制时，只能有2个通道同时设置为调制输出；
							8电平和16电平调制时，只能有1个通道同时设置为调制输出。请适当设置几电平调制以满足应用需求。

**注意！！！：设置成4电平调制时，输出通道只能选择CH0-1
							设置成8,16电平调制时，输出通道只能选择CH0
							本函数未做任意通道兼容，具体方法请参考AD9959芯片手册22-23页，操作FR1[14:12]为对应值。
**************************************************************/
void AD9959_Modulation_Init(uint8_t Channel,uint8_t Modulation,uint8_t Sweep_en,uint8_t Nlevel)
{
	uint8_t i=0;
	uint8_t CHANNEL[1]={0x00};
	uint8_t FR1_data[3];
	uint8_t FR2_data[2];
	uint8_t CFR_data[3];
	for(i=0;i<3;i++)//设置默认值
	{
		FR1_data[i]=FR1_DATA[i];
		CFR_data[i]=CFR_DATA[i];
	}
	FR2_data[0]=FR2_DATA[0];
	FR2_data[1]=FR2_DATA[1];
		
	CHANNEL[0]=Channel;
	AD9959_WriteData(CSR_ADD,1,CHANNEL); //控制寄存器写入CHn通道，选择CHn；以下设置均针对CHn
	
	FR1_data[1]=Nlevel;
	CFR_data[0]=Modulation;
	CFR_data[1]|=Sweep_en;
	CFR_data[2]=0x00;

	if(Channel!=0)//有通道被使能
	{
		AD9959_WriteData(FR1_ADD,3,FR1_data);//写功能寄存器1
		AD9959_WriteData(FR2_ADD,2,FR2_data);//写功能寄存器1
		AD9959_WriteData(CFR_ADD,3,CFR_data);//写通道功能寄存器
	}
}


/************************************************************
** 函数名称 ：void AD9959_SetFSK(uint8_t Channel, uint32_t *data,uint16_t Phase)
** 函数功能 ：设置FSK调制的参数
** 入口参数 ：Channel:  输出通道 CH0-CH3
							*data:	调整频率数据的起始地址
							Phase:	输出相位,范围：0~16383(对应角度：0°~360°)
** 出口参数 ：无
** 函数说明 ：FSK时信号幅度默认为最大
**************************************************************/
void AD9959_SetFSK(uint8_t Channel, uint32_t *data,uint16_t Phase)
{
	uint8_t i=0;
	uint8_t CHANNEL[1]={0x00};	
	
	CHANNEL[0]=Channel;
	AD9959_WriteData(CSR_ADD,1,CHANNEL); //控制寄存器写入CHn通道，选择CHn；以下设置均针对CHn
	Write_CPOW0(Phase);//设置相位
	
	Write_CFTW0(data[0]);
	for(i=0;i<15;i++)
		Write_Profile_Fre(i, data[i+1]);	
}

/************************************************************
** 函数名称 ：void AD9959_SetASK(uint8_t Channel, uint32_t *data,uint32_t fre,uint16_t Phase)
** 函数功能 ：设置ASK调制的参数
** 入口参数 ：Channel:  输出通道 CH0-CH3
							*data: 调整幅度数据的起始地址
							Freq:		输出频率，范围0~200 000 000 Hz
							Phase:	输出相位,范围：0~16383(对应角度：0°~360°)
** 出口参数 ：无
** 函数说明 ：无
**************************************************************/
void AD9959_SetASK(uint8_t Channel, uint16_t *data,uint32_t fre,uint16_t Phase)
{
	uint8_t i=0;
	uint8_t CHANNEL[1]={0x00};	
	
	CHANNEL[0]=Channel;
	AD9959_WriteData(CSR_ADD,1,CHANNEL); //控制寄存器写入CHn通道，选择CHn；以下设置均针对CHn
	Write_CFTW0(fre);//设置频率
	Write_CPOW0(Phase);//设置相位
	
	Write_ACR(data[0]);
	for(i=0;i<15;i++)
		Write_Profile_Ampli(i, data[i+1]);	
}

/************************************************************
** 函数名称 ：void AD9959_SetPSK(uint8_t Channel, uint16_t *data,uint32_t fre,uint16_t Phase)
** 函数功能 ：设置PSK调制的参数
** 入口参数 ：Channel:  输出通道 CH0-CH3
							*data:	调整相位数据的起始地址
							Freq:		输出频率，范围0~200 000 000 Hz
** 出口参数 ：无
** 函数说明 ：无
**************************************************************/
void AD9959_SetPSK(uint8_t Channel, uint16_t *data,uint32_t Freq)
{
	uint8_t i=0;
	uint8_t CHANNEL[1]={0x00};	
	
	CHANNEL[0]=Channel;
	AD9959_WriteData(CSR_ADD,1,CHANNEL); //控制寄存器写入CHn通道，选择CHn；以下设置均针对CHn
	Write_CFTW0(Freq);
	
	Write_CPOW0(data[0]);
	for(i=0;i<15;i++)
		Write_Profile_Phase(i, data[i+1]);	
}

/************************************************************
** 函数名称 ：void AD9959_SetFre_Sweep(uint8_t Channel, uint32_t s_data,uint32_t e_data,uint8_t fsrr,uint8_t rsrr,uint32_t r_delta,uint32_t f_delta,uint16_t Phase)
** 函数功能 ：设置线性扫频的参数
** 入口参数 ：Channel:  输出通道 CH0-CH3
							s_data:	起始频率，范围0~200 000 000 Hz
							e_data:	结束频率，范围0~200 000 000 Hz
							r_delta:上升步长频率,0~200 000 000Hz
							f_delta:下降步长频率,0~200 000 000Hz

							rsrr:		上升斜率,范围：1~255，系统时钟为500Mhz时一个控制字约为8ns
							fsrr:		下降斜率,范围：1~255
							Ampli:	输出幅度,范围0~1023，控制值0~1023对应输出幅度0~500mVpp左右
							Phase:	输出相位,范围：0~16383(对应角度：0°~360°)
** 出口参数 ：无
** 函数说明 ：频点与频点间停留时间 dT = Xsrr*8 单位ns，扫描点数=(起始-结束)/步长
							扫频总时间=总扫描频点数*dT
**************************************************************/
void AD9959_SetFre_Sweep(uint8_t Channel, uint32_t s_data,uint32_t e_data,uint32_t r_delta,uint32_t f_delta,uint8_t rsrr,uint8_t fsrr,uint16_t Ampli,uint16_t Phase)
{
	uint8_t CHANNEL[1]={0x00};
	uint32_t Fer_data=0;            
	
	CHANNEL[0]=Channel;
	AD9959_WriteData(CSR_ADD,1,CHANNEL); //控制寄存器写入CHn通道，选择CHn；以下设置均针对CHn
	
	Write_CPOW0(Phase);//设置相位
	Write_ACR(Ampli); //幅度设置
	
	Write_LSRR(rsrr,fsrr);//斜率
	
	Fer_data=(uint32_t)r_delta*ACC_FRE_FACTOR;	 //频率转换成控制字
	Write_RDW(Fer_data);//上升步长
	
	Fer_data=(uint32_t)f_delta*ACC_FRE_FACTOR;
	Write_FDW(Fer_data);//下降步长
	
	Write_CFTW0(s_data);//起始频率
	Write_Profile_Fre(0, e_data);//结束频率
}

/************************************************************
** 函数名称 ：void AD9959_SetAmp_Sweep(uint8_t Channel, uint32_t s_Ampli,uint16_t e_Ampli,uint32_t r_delta,uint32_t f_delta,uint8_t rsrr,uint8_t fsrr,uint32_t fre,uint16_t Phase)
** 函数功能 ：设置线性扫幅的参数
** 入口参数 ：Channel:  输出通道 CH0-CH3
							s_Ampli:	起始幅度，控制值0~1023对应输出幅度0~500mVpp左右
							e_Ampli:	结束幅度，
							
							r_delta:	上升步长幅度,0~1023
							f_delta:	下降步长幅度,0~1023

							rsrr:			上升斜率,范围：1~255，系统时钟为500Mhz时一个控制字约为8ns
							fsrr:			下降斜率,范围：1~255

							fre:			输出频率，范围0~200 000 000 Hz
							Phase:		输出相位,范围：0~16383(对应角度：0°~360°)
** 出口参数 ：无
** 函数说明 ：幅点与幅点间停留时间 dT = Xsrr*8 单位ns；扫描点数=(起始-结束)/步长
							扫幅总时间=总扫描幅点数*dT
**************************************************************/
void AD9959_SetAmp_Sweep(uint8_t Channel, uint32_t s_Ampli,uint16_t e_Ampli,uint32_t r_delta,uint32_t f_delta,uint8_t rsrr,uint8_t fsrr,uint32_t fre,uint16_t Phase)
{
	uint8_t CHANNEL[1]={0x00};
	uint8_t ACR_data[3] = {0x00,0x00,0x00};
		
	CHANNEL[0]=Channel;
	AD9959_WriteData(CSR_ADD,1,CHANNEL); //控制寄存器写入CHn通道，选择CHn；以下设置均针对CHn
	
	Write_CFTW0(fre); //幅度频率
	Write_CPOW0(Phase);//设置相位
	
	Write_LSRR(rsrr,fsrr);//斜率
	
	Write_RDW(r_delta<<22);//上升步长
	
	Write_FDW(f_delta<<22);//下降步长

	ACR_data[1] = (uint8_t)(s_Ampli>>8); //高位数据
	ACR_data[2] = (uint8_t)s_Ampli;  //低位数据
  AD9959_WriteData(ACR_ADD,3,ACR_data); //ACR 设定起始幅度
					
	Write_Profile_Ampli(0, e_Ampli);//结束幅度
}


/************************************************************
** 函数名称 ：void AD9959_SetPhase_Sweep(uint8_t Channel, uint16_t s_data,uint16_t e_data,uint16_t r_delta,uint16_t f_delta,uint8_t rsrr,uint8_t fsrr,uint32_t fre,uint16_t Ampli)
** 函数功能 ：设置线性扫相的参数
** 入口参数 ：Channel:  输出通道 CH0-CH3
							s_data:	起始相位，范围：0~16383(对应角度：0°~360°)
							e_data:	结束相位，
							r_delta:上升步长,范围：0~16383(对应角度：0°~360°)
							f_delta:下降步长,

							rsrr:		上升斜率,范围：1~255，系统时钟为500Mhz时一个控制字约为8ns
							fsrr:		下降斜率,范围：1~255
							fre:		输出频率，范围0~200 000 000 Hz
							Ampli:	输出幅度,范围0~1023，控制值0~1023对应输出幅度0~500mVpp左右
** 出口参数 ：无
** 函数说明 ：频点与频点间停留时间 dT = Xsrr*8 单位ns；扫描点数=(起始-结束)/步长
							扫频总时间=总扫描频点数*dT
**************************************************************/
void AD9959_SetPhase_Sweep(uint8_t Channel, uint16_t s_data,uint16_t e_data,uint16_t r_delta,uint16_t f_delta,uint8_t rsrr,uint8_t fsrr,uint32_t fre,uint16_t Ampli)
{
	uint8_t CHANNEL[1]={0x00};
	
	CHANNEL[0]=Channel;
	AD9959_WriteData(CSR_ADD,1,CHANNEL); //控制寄存器写入CHn通道，选择CHn；以下设置均针对CHn
	
	Write_CFTW0(fre); //幅度频率
	Write_ACR(Ampli); //幅度设置
	
	Write_LSRR(rsrr,fsrr);//斜率
	
	Write_RDW(r_delta<<18);//上升步长
	
	Write_FDW(f_delta<<18);//下降步长
	
	Write_CPOW0(s_data);//起始相位
	Write_Profile_Phase(0, e_data);//结束相位
}








