#include "AD7190.h"
/*******************************
向SPI器件写入一个字节数据
*******************************/
AD7190::AD7190()
{
	delay_init(168);
	AD7190_Init();
}

AD7190::~AD7190()
{
	
}

void AD7190::IO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_CS1|RCC_SPI_CLK|RCC_SPI_DIN|RCC_SPI_DOUT,ENABLE);

	GPIO_InitStructure.GPIO_Pin=PIN_SPI_DOUT;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_Init(GPIO_SPI_DOUT,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin=PIN_SPI_CLK;
	GPIO_Init(GPIO_SPI_CLK,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin=PIN_CS1;
	GPIO_Init(GPIO_CS1,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin=PIN_SPI_DIN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_Init(GPIO_SPI_DIN,&GPIO_InitStructure);

	CS = 0;
	SCLK = 1;	
}

void AD7190::AD7190_Init(void)
{
	IO_Init();
  
	CS = 0;
	
	data_write(0xff);
	data_write(0xff);
	data_write(0xff);
	data_write(0xff);
	data_write(0xff);	//连续写40个1进行初始化	         
	delay_us(500);		//需要等待至少500us
	
	data_write(0x10);	//选中配置寄存器
	data_write(0x80);	//斩波使能；选择在REFIN1(+)与REFIN1(-)之间施加外部基准电压
	data_write(0x00);	//选择通道CH0：AIN1(+)与AIN2(-)
	data_write(0x00);	//激励电流关闭，缓冲器使能，双极性工作，增益gain=1(ADC输入范围正负2.5V)

	data_write(0x08);	//选中模式寄存器
	data_write(0x88);	//内部零电平校准
	data_write(0x00);
	data_write(0x20); 

	data_write(0x08);	//选中模式寄存器
	data_write(0xA8);	//内部满量程校准
	data_write(0x00);
	data_write(0x20);	 

	data_write(0x08);	//选中模式寄存器
	data_write(0x08);	//4.92MHz内部时钟
	data_write(0x00);
	data_write(0x20);	//输出速率

	DOUT = 1;
}

void AD7190::data_write(u8 data)
{
	for (u8 i=0;i<8;i++)
	{
		if(data & 0x80)
		{
			DOUT = 1;
		}	
		else 
		{
			DOUT = 0;
		}	
		SCLK = 0;
		data=(data<<1);
		SCLK = 1;
	}
}

unsigned char AD7190::data_read(void)
{
	u8 data=0;
	for (u8 i=0;i<8;i++)
	{
		SCLK = 0;
		data = (data<<1);
		SCLK = 1;
		if(DIN == 1)
		{
			data |= 0x01;
		}
		else 
		{
			data &= ~0x01;
		}
	}
	return data;
}

u32 AD7190::AD7190_Read_Data(u16 ID)
{ 
	u32  AD_result = 0;
	data_write(ID);
	delay_us(1);

	while(DIN == 1);/*！！！！*/

	AD_result = data_read();
	AD_result <<= 8;
	AD_result = data_read()+AD_result;  
	AD_result <<= 8;
	AD_result = data_read()+AD_result;
	
	return   AD_result;
} 

u32 AD7190::Get_Register_Val(void)
{
	return AD7190_Read_Data(0x58);
}

double AD7190::Get_Voltage(void)
 {
	double i = Get_Register_Val();
	if(i >= 16777216/2)
	{
		i = i - 16777216/2.0;
		i = i*3.3/(16777216/2.0);
		return i;
	}
	else
	{
		i = AD7190_ACC/2.0-i;
		i = i*3.3/(AD7190_ACC/2.0);
		return 0-i;
	}
}




