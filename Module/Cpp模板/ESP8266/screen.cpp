#include "screen.h"

screen::screen(u32 baudrate)
{
	uart_init(baudrate);
}

screen::~screen()
{
	
}

void screen::update(u8 vol,u8 channel)
{
	u8 suffer[3] = {0xff,0xff,0xff};
	u8 Tx_data_counter = 0;
	printf("add 1,%d,%d",channel,vol);
	while(1)
	{
		if((USART1->SR & (1<<7))) //·¢ËÍÊı¾İ¼Ä´æÆ÷¿Õ
		{
			USART1->DR = suffer[Tx_data_counter];
			Tx_data_counter++;
			if(Tx_data_counter == 3)
			{
				Tx_data_counter = 0;
				break;
			}
		}		
	}	
}

void screen::clear_screen(void)
{
	u8 suffer[3] = {0xff,0xff,0xff};
	u8 Tx_data_counter = 0;
	printf("cle 1,255");
	while(1)
	{
		if((USART1->SR & (1<<7))) //·¢ËÍÊı¾İ¼Ä´æÆ÷¿Õ
		{
			USART1->DR = suffer[Tx_data_counter];
			Tx_data_counter++;
			if(Tx_data_counter == 3)
			{
				Tx_data_counter = 0;
				break;
			}
		}		
	}	
}

void screen::txt_send(u16 val,const char* id,const char* name)
{
	u8 suffer[3] = {0xff,0xff,0xff};
	u8 Tx_data_counter = 0;
	//printf("add 1,0,%d",VOLTAGE/16);
	printf("%s.txt=\"%d %s\"",id,val,name);
	while(1)
	{
		if((USART1->SR & (1<<7))) //·¢ËÍÊı¾İ¼Ä´æÆ÷¿Õ
		{
			USART1->DR = suffer[Tx_data_counter];
			Tx_data_counter++;
			if(Tx_data_counter == 3)
			{
				Tx_data_counter = 0;
				break;
			}
		}		
	}	
}

void screen::info_send(const char* info,const char* id)
{
	u8 suffer[3] = {0xff,0xff,0xff};
	u8 Tx_data_counter = 0;
	printf("%s.txt=\"%s\"",id,info);
	while(1)
	{
		if((USART1->SR & (1<<7))) //·¢ËÍÊı¾İ¼Ä´æÆ÷¿Õ
		{
			USART1->DR = suffer[Tx_data_counter];
			Tx_data_counter++;
			if(Tx_data_counter == 3)
			{
				Tx_data_counter = 0;
				break;
			}
		}		
	}	
}
