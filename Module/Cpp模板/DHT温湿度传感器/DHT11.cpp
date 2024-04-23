#include "DHT11.h"

DHT11::DHT11()
{
	delay_init(168);

}

DHT11::~DHT11()
{
	
}

void DHT11::DHT11_GPIO_Output()
{
	GPIO_InitTypeDef	DHT_PORT;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	
	DHT_PORT.GPIO_Mode	= GPIO_Mode_OUT;
	DHT_PORT.GPIO_OType = GPIO_OType_PP;
	DHT_PORT.GPIO_Pin	= GPIO_Pin_0;
	DHT_PORT.GPIO_PuPd	= GPIO_PuPd_UP;
	DHT_PORT.GPIO_Speed	= GPIO_Speed_100MHz;
	
	GPIO_Init(GPIOE,&DHT_PORT);
}

void DHT11::DHT11_GPIO_Input()
{
	GPIO_InitTypeDef	DHT_PORT;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	
	DHT_PORT.GPIO_Mode	= GPIO_Mode_IN;
	DHT_PORT.GPIO_Pin	= GPIO_Pin_0;
	DHT_PORT.GPIO_Speed	= GPIO_Speed_100MHz;
	
	GPIO_Init(GPIOE,&DHT_PORT);
}

void DHT11::DHT11_reset()
{
	DHT11_GPIO_Output();
	DHT11_OUT_L;
	delay_us(19000);
	DHT11_OUT_H;
	delay_us(30);
	DHT11_GPIO_Input();
}

u16 DHT11::DHT11_Scan()
{
	return DHT11_IN;
}

u16 DHT11::DHT11_Read_Bit()
{
	while(DHT11_IN == RESET);
	delay_us(40);
	if(DHT11_IN == SET)
	{
		while(DHT11_IN == SET);
		return 1;
	}
	else
	{
		return 0;
	}
}

u16 DHT11::DHT11_Read_Byte()
{
	u16	i;
	u16	data = 0;
	
	for(i=0;i<8;i++)
	{
		data <<= 1;
		data |= DHT11_Read_Bit();
	}
	
	return data;
}

u16 DHT11::DHT11_Read_Data(u8 buffer[5])
{
	u16	i = 0;
	
	DHT11_reset();
	if(DHT11_Scan() == RESET)
	{
		while(DHT11_Scan() == RESET);
		while(DHT11_Scan() == SET);
		
		for(i=0;i<5;i++)
		{
			buffer[i] = DHT11_Read_Byte();
		}
		
		while(DHT11_Scan() == RESET);
		DHT11_GPIO_Output();
		DHT11_OUT_H;
		
		u8 checksum = buffer[0]+buffer[1]+buffer[2]+buffer[3];
		if(checksum != buffer[4])
		{
			return 1;
		}
	}
	
	return 0;
}

void DHT11::Func_Activate()
{
	u8 buffer[5];
		
	if(DHT11_Read_Data(buffer) == 0)
	{
		hum =  buffer[0]+buffer[1] / 10.0;
		temp = buffer[2]+buffer[3] / 10.0;
		sprintf(temp_value,"%f",temp);
	}
	
	LCD_ShowString(10,50,70,16,16,"humidity:");
	LCD_ShowString(120,50,20,16,16,"%");
	LCD_ShowNum(90,50,hum,3,16);
	LCD_ShowString(10,20,40,16,16,"temp:");
	LCD_ShowString(100,20,20,16,16,"C");
	LCD_ShowString(60,20,30,16,16,temp_value);	
}
