#include "AD9959.h"

unsigned char AD9959_CSR_DATA[1] = {0x00};
unsigned char AD9959_CSR_DATA0[1] = {0x10};
unsigned char AD9959_CSR_DATA1[2] = {0x20};
unsigned char AD9959_CSR_DATA2[3] = {0x40};
unsigned char AD9959_CSR_DATA3[4] = {0x80};
unsigned char AD9959_FR1_DATA[3] = {0xD0, 0x00, 0x00};
unsigned char AD9959_FR2_DATA[2] = {0x00, 0x00};
unsigned char AD9959_CFR_DATA[3] = {0x00, 0x03, 0x02};
unsigned char AD9959_CFTW0_DATA[4] = {0x00, 0x00, 0x00, 0x00};
unsigned char AD9959_CFTW0_DATA0[4] = {0x33,0x33,0x33,0x33}; //100MHz;
unsigned char AD9959_CFTW0_DATA1[4] = {0x28,0xF5,0xC2,0x8F}; //80MHz;
unsigned char AD9959_CFTW0_DATA2[4] = {0x05,0x1E,0xB8,0x52}; //10MHz;
unsigned char AD9959_CFTW0_DATA3[4] = {0x00,0x83,0x12,0x6F}; //1MHz;
unsigned char AD9959_CPOW0_DATA[2] = {0x00, 0x00};
unsigned char AD9959_ACR_DATA[3] = {0x00, 0x00, 0x00};
unsigned char AD9959_LSRR_DATA[2] = {0x00, 0x00};
unsigned char AD9959_RDW_DATA[4] = {0x00, 0x00, 0x00, 0x00};
unsigned char AD9959_FDW_DATA[4] = {0x00, 0x00, 0x00, 0x00};

void AD9959_Delay(void)
{
	unsigned int i = 500;
	while(i--);
}

void AD9959_SDA_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin = AD9959_SDIO0_Pin;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(AD9959_SDIO0_GPIO, &GPIO_InitStructure);
}

void AD9959_SDA_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_Pin = AD9959_SDIO0_Pin;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(AD9959_SDIO0_GPIO, &GPIO_InitStructure);
}

void AD9959_UpDate(void)
{
	GPIO_ResetBits(AD9959_UPDATE_GPIO, AD9959_UPDATE_Pin);
	AD9959_Delay();
	GPIO_SetBits(AD9959_UPDATE_GPIO, AD9959_UPDATE_Pin);
	GPIO_ResetBits(AD9959_UPDATE_GPIO, AD9959_UPDATE_Pin);
}

void AD9959_RST(void)
{
	unsigned int i;
	GPIO_ResetBits(AD9959_RST_GPIO, AD9959_RST_Pin);
	for(i=0;i<500;i++);
	GPIO_SetBits(AD9959_RST_GPIO, AD9959_RST_Pin);
	for(i=0;i<500;i++);
	GPIO_ResetBits(AD9959_RST_GPIO, AD9959_RST_Pin);
}

void AD9959_WriteByte(unsigned char NewByte)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		GPIO_ResetBits(AD9959_SCLK_GPIO, AD9959_SCLK_Pin);
		AD9959_Delay();
		if(NewByte & 0x80)
			GPIO_SetBits(AD9959_SDIO0_GPIO, AD9959_SDIO0_Pin);
		else
			GPIO_ResetBits(AD9959_SDIO0_GPIO, AD9959_SDIO0_Pin);
		AD9959_Delay();
		GPIO_SetBits(AD9959_SCLK_GPIO, AD9959_SCLK_Pin);
		NewByte <<= 1;
		AD9959_Delay();
	}
	GPIO_ResetBits(AD9959_SCLK_GPIO, AD9959_SCLK_Pin);
}

void AD9959_WriteData(unsigned char RegAddr, unsigned char NumberOfRegs, unsigned char *RegData, unsigned char up)
{
	unsigned char RegIndex = 0;
	unsigned char i = 0;
	
	GPIO_SetBits(AD9959_CS_GPIO, AD9959_CS_Pin);
	GPIO_ResetBits(AD9959_SCLK_GPIO, AD9959_SCLK_Pin);
	GPIO_ResetBits(AD9959_CS_GPIO, AD9959_CS_Pin);
	AD9959_Delay();
	
	AD9959_SDA_OUT();
	
	AD9959_WriteByte(RegAddr);
	for(RegIndex=0;RegIndex<NumberOfRegs;RegIndex++)
	{
		AD9959_WriteByte(RegData[RegIndex]);
	}
	if(up)
		AD9959_UpDate();
	
	AD9959_Delay();
	GPIO_SetBits(AD9959_CS_GPIO, AD9959_CS_Pin);
}


void AD9959_ReadData(unsigned char RegAddr, unsigned char NumberOfRegs, unsigned char *RegData)
{
	unsigned	char	ControlValue = 0;
	signed		char	RegisterIndex = 0;
	unsigned	char	ReceiveData = 0;
	unsigned	char	i = 0;
	unsigned	int		iTemp = 0;
	
	GPIO_SetBits(AD9959_CS_GPIO, AD9959_CS_Pin);
	GPIO_SetBits(AD9959_SCLK_GPIO, AD9959_SCLK_Pin);
	GPIO_ResetBits(AD9959_CS_GPIO, AD9959_CS_Pin);
	
	ControlValue = RegAddr;
 	AD9959_SDA_OUT();
	for(i=0; i<8; i++)
	{
		GPIO_ResetBits(AD9959_SCLK_GPIO, AD9959_SCLK_Pin);
		if(0x80 == (ControlValue & 0x80))
		{
			GPIO_SetBits(AD9959_SDIO0_GPIO, AD9959_SDIO0_Pin);
		}
		else
		{
			GPIO_ResetBits(AD9959_SDIO0_GPIO, AD9959_SDIO0_Pin);
		}
		GPIO_SetBits(AD9959_SCLK_GPIO, AD9959_SCLK_Pin);
		ControlValue <<= 1;	//Rotate data
	}

	AD9959_SDA_IN();	//Make SDA an input

	//Read data in
	for (RegisterIndex=NumberOfRegs; RegisterIndex>0; RegisterIndex--)
	{
		for(i=0; i<8; i++)
		{
			GPIO_ResetBits(AD9959_SCLK_GPIO, AD9959_SCLK_Pin);
			ReceiveData <<= 1;		//Rotate data
			iTemp = GPIO_ReadInputDataBit(AD9959_SDIO0_GPIO, AD9959_SDIO0_Pin);			//Read SDIO of AD9959
			if(iTemp)
			{
				ReceiveData |= 1;
			}
			GPIO_SetBits(AD9959_SCLK_GPIO, AD9959_SCLK_Pin);
		}
		*(RegData + RegisterIndex - 1) = ReceiveData;
	}
	GPIO_SetBits(AD9959_CS_GPIO, AD9959_CS_Pin);
}



void AD9959_Init_GPIO(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd( AD9959_CS_RCC_GPIO      | 
													AD9959_SCLK_RCC_GPIO    |
													AD9959_SDIO0_RCC_GPIO   |
													AD9959_SDIO1_RCC_GPIO   |
													AD9959_SDIO2_RCC_GPIO   |
													AD9959_SDIO3_RCC_GPIO   |
													AD9959_UPDATE_RCC_GPIO  |
													AD9959_PS0_RCC_GPIO     |
													AD9959_PS1_RCC_GPIO     |
													AD9959_PS2_RCC_GPIO     |
													AD9959_PS3_RCC_GPIO     |
													AD9959_RST_RCC_GPIO     |
													AD9959_PWD_RCC_GPIO     ,
													ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = AD9959_CS_Pin;
	GPIO_Init(AD9959_CS_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = AD9959_SCLK_Pin;
	GPIO_Init(AD9959_SCLK_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = AD9959_SDIO0_Pin;
	GPIO_Init(AD9959_SDIO0_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = AD9959_SDIO1_Pin;
	GPIO_Init(AD9959_SDIO1_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = AD9959_SDIO2_Pin;
	GPIO_Init(AD9959_SDIO2_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = AD9959_SDIO3_Pin;
	GPIO_Init(AD9959_SDIO3_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = AD9959_UPDATE_Pin;
	GPIO_Init(AD9959_UPDATE_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = AD9959_PS0_Pin;
	GPIO_Init(AD9959_PS0_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = AD9959_PS1_Pin;
	GPIO_Init(AD9959_PS1_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = AD9959_PS2_Pin;
	GPIO_Init(AD9959_PS2_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = AD9959_PS3_Pin;
	GPIO_Init(AD9959_PS3_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = AD9959_RST_Pin;
	GPIO_Init(AD9959_RST_GPIO, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = AD9959_PWD_Pin;
	GPIO_Init(AD9959_PWD_GPIO, &GPIO_InitStructure);
}

void AD9959_Init(void)
{
	static unsigned char tempData[4]= {0x00};

	AD9959_Init_GPIO();
	AD9959_RST();
	
	AD9959_ReadData(0x00, 1, tempData);
	
	AD9959_WriteData(AD9959_FR1_ADD, 3, AD9959_FR1_DATA, 1);
	AD9959_WriteData(AD9959_CFTW0_ADD, 4, AD9959_CFTW0_DATA0, 1);
	
	//控制寄存器写入CH0通道    输出CH0设定频率
	AD9959_WriteData(AD9959_CSR_ADD, 1, AD9959_CSR_DATA0, 1);
	AD9959_WriteData(AD9959_CFTW0_ADD, 4, AD9959_CFTW0_DATA0, 1);
	
	//控制寄存器写入CH1通道    输出CH1设定频率
	AD9959_WriteData(AD9959_CSR_ADD, 1, AD9959_CSR_DATA1, 1);
	AD9959_WriteData(AD9959_CFTW0_ADD, 4, AD9959_CFTW0_DATA1, 1);
	
	//控制寄存器写入CH2通道    输出CH2设定频率
	AD9959_WriteData(AD9959_CSR_ADD, 1, AD9959_CSR_DATA2, 1);
	AD9959_WriteData(AD9959_CFTW0_ADD, 4, AD9959_CFTW0_DATA2, 1);
	
	//控制寄存器写入CH3通道    输出CH3设定频率
	AD9959_WriteData(AD9959_CSR_ADD, 1, AD9959_CSR_DATA3, 1);
	AD9959_WriteData(AD9959_CFTW0_ADD, 4, AD9959_CFTW0_DATA3, 1);
}

void AD9959_WriteFreq(unsigned char CH, double Freq)
{
	unsigned char CFTW0_DATA[4] = {0x00, 0x00, 0x00, 0x00};
	unsigned long Temp;
	
	Temp = (unsigned long)(Freq * 8.589934592);  //2^32 /500M
	CFTW0_DATA[3] = Temp >> 0;
	CFTW0_DATA[2] = Temp >> 8;
	CFTW0_DATA[1] = Temp >> 16;
	CFTW0_DATA[0] = Temp >> 24;
	switch(CH)
	{
		case 0:
			AD9959_WriteData(AD9959_CSR_ADD, 1, AD9959_CSR_DATA0, 1);
			AD9959_WriteData(AD9959_CFTW0_ADD, 4, CFTW0_DATA, 1);
			break;
		case 1:
			AD9959_WriteData(AD9959_CSR_ADD, 1, AD9959_CSR_DATA1, 1);
			AD9959_WriteData(AD9959_CFTW0_ADD, 4, CFTW0_DATA, 1);
			break;
		case 2:
			AD9959_WriteData(AD9959_CSR_ADD, 1, AD9959_CSR_DATA2, 1);
			AD9959_WriteData(AD9959_CFTW0_ADD, 4, CFTW0_DATA, 1);
			break;
		case 3:
			AD9959_WriteData(AD9959_CSR_ADD, 1, AD9959_CSR_DATA3, 1);
			AD9959_WriteData(AD9959_CFTW0_ADD, 4, CFTW0_DATA, 1);
			break;
	}
}

void AD9959_WriteAmplitude(unsigned char CH, unsigned int Amplitude)
{
	unsigned int A_temp;
	A_temp = Amplitude | 0x1000;
	AD9959_ACR_DATA[2] = (unsigned char)A_temp;  //低位数据
	AD9959_ACR_DATA[1] = (unsigned char)(A_temp >> 8); //高位数据
	switch(CH)
	{
		case 0:
			AD9959_WriteData(AD9959_CSR_ADD, 1, AD9959_CSR_DATA0, 1);
			AD9959_WriteData(AD9959_ACR_ADD, 3, AD9959_ACR_DATA, 1);
			break;
		case 1:
			AD9959_WriteData(AD9959_CSR_ADD, 1, AD9959_CSR_DATA1, 1);
			AD9959_WriteData(AD9959_ACR_ADD, 3, AD9959_ACR_DATA, 1);
			break;
		case 2:
			AD9959_WriteData(AD9959_CSR_ADD, 1, AD9959_CSR_DATA2, 1);
			AD9959_WriteData(AD9959_ACR_ADD, 3, AD9959_ACR_DATA, 1);
			break;
		case 3:
			AD9959_WriteData(AD9959_CSR_ADD, 1, AD9959_CSR_DATA3, 1);
			AD9959_WriteData(AD9959_ACR_ADD, 3, AD9959_ACR_DATA, 1);
			break;
	}
}

void AD9959_Write_Phase(unsigned char CH, unsigned int Phase)
{
	unsigned int P_temp =0;
	P_temp = (unsigned int)Phase * 45.511111; //2^14/360
	AD9959_CPOW0_DATA[1] = (unsigned char)P_temp;
	AD9959_CPOW0_DATA[0] = (unsigned char)(P_temp>8);
	switch(CH)
	{
		AD9959_WriteData(AD9959_CSR_ADD, 1, AD9959_CSR_DATA0, 0);
		AD9959_WriteData(AD9959_CPOW0_ADD, 2, AD9959_CPOW0_DATA, 0);
	}
}

