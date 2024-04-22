/**
  **************************************************************************************
  * @file ad7606_hal.c
  * @author shaokairu
  * @date 2021.8.2
  * @brief This file provides the function of ad7606 board 
  **************************************************************************************
  */

/* Includes --------------------------------------------------------------------------*/
#include "AD7606_hal.h"
#include "delay.h"

/* privated constants ----------------------------------------------------------------*/
#define sampling_0times 0 
#define sampling_2times 1 
#define sampling_4times 2 
#define sampling_8times 3 
#define sampling_16times 4 
#define sampling_32times 5 
#define sampling_64times 6

/** @defgroup GPIO
  * @{
  */
  
/**
  * @brief AD7606 serial-port-control pins configuration
  * @param None
  * @retval None
  * @note the clock must be enabled when the gpio changed
  */
void AD7606_serial_GPIO_configuration(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    /* enable the clock */
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
   
    /* set gpio */
    GPIO_Initure.Pin = CS_PIN;
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Initure.Pull = GPIO_PULLUP;
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(CS_GPIO, &GPIO_Initure);

    GPIO_Initure.Pin = CLK_PIN;
    HAL_GPIO_Init(CLK_GPIO, &GPIO_Initure);

    GPIO_Initure.Pin = RST_PIN;
    HAL_GPIO_Init(RST_GPIO, &GPIO_Initure);

    GPIO_Initure.Pin = CONA_PIN;
    HAL_GPIO_Init(CONB_GPIO, &GPIO_Initure);

    GPIO_Initure.Pin = CONB_PIN;
    HAL_GPIO_Init(CONB_GPIO, &GPIO_Initure);

    GPIO_Initure.Pin = STBY_PIN;
    HAL_GPIO_Init(STBY_GPIO, &GPIO_Initure);

    GPIO_Initure.Pin = OS10_PIN;
    HAL_GPIO_Init(OS10_GPIO, &GPIO_Initure);

    GPIO_Initure.Pin = OS11_PIN;
    HAL_GPIO_Init(OS11_GPIO, &GPIO_Initure);

    GPIO_Initure.Pin = OS12_PIN;
    HAL_GPIO_Init(OS12_GPIO, &GPIO_Initure);

    GPIO_Initure.Pin = FDA_PIN;
    GPIO_Initure.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(FDA_GPIO, &GPIO_Initure);

    GPIO_Initure.Pin = BUSY_PIN;
    HAL_GPIO_Init(BUSY_GPIO, &GPIO_Initure);

    GPIO_Initure.Pin = DAT_PIN;
    HAL_GPIO_Init(DAT_GPIO, &GPIO_Initure);
}

/**
  * @}
  */

/** @defgroup AD7606 driver
  * @{
  */

/**
  * @brief reset the AD7606
  * @param None
  * @retval None
  */
void AD7606_reset(void) 
{ 
	rst_Reset;
	delay_us(1);
	rst_Set; 
	delay_us(1);
	rst_Reset; 
}  

/**
  * @brief control AD7606 to start simultaneous sampling of all analog input channels.
  * @param None
  * @retval None
  */
void AD7606_startconvst(void)
{  
	convstA_Reset;	
	convstB_Reset;	
	delay_us(1);
	convstA_Set;
	convstB_Set;
}

/**
  * @brief AD7606 working parameter configuration
  * @param None
  * @retval None
  */
void AD7606_init(void)
{
    convstA_Set;
    convstB_Set;
    delay_ms(1);
    STby_Set;
    clk_Set;
    
    /* disable the low pass filter */
    OS10_Reset;
    OS11_Reset;
    OS12_Reset;
    
    AD7606_reset();
}

/** @defgroup serial control
  * @{
  */

/**
  * @brief recive the value of the ADC sampling
  * @param DB_data: the array storing the value of the ADC sampling
  * @retval None
  */
void AD7606_serial_read_data(int16_t * DB_data) 
{  
	uint8_t i,j; 
	
    /* eight counts were received */
	for(i=0;i<8;i++)  
	{
		uint16_t DB_data1 = 0;
        
		cs_Reset; 
	    delay_us(1);
        
        /* SPI recive 16 bit */
		for(j=0;j<16;j++)
		{		
            clk_Reset;
            delay_us(1);
            DB_data1 = ((uint16_t)(HAL_GPIO_ReadPin(DAT_GPIO, DAT_PIN)) << (15 - j)) + DB_data1;
            delay_us(1);
            clk_Set;
            delay_us(1);				
		}		
		cs_Set;	
        
		DB_data[i] = (uint16_t)DB_data1;
	}	
} 

void AD7606_GetAnalogValue(int16_t *data,float *analog_data)
{
	uint8_t i;
	for(i=0;i<8;i++)
	{
		*(analog_data+i)=*(data+i);
		*(analog_data+i)=*(analog_data+i)*range/32768;
	}
}

void adcgetvalue(int16_t *adcdat,uint8_t times)
{
	int16_t ad7606_dat[8];

	int i,j,sum[8]={0,0,0,0,0,0,0,0};
	for(i=0;i<times;i++)
	{
		AD7606_startconvst();
		delay_ms(5);
		AD7606_serial_read_data(ad7606_dat);
		for(j=0;j<8;j++)
		{
			sum[j] = sum[j]+ad7606_dat[j];
		}
	}
	for(i=0;i<8;i++)
	{
		ad7606_dat[i]=(float)sum[i]/times;		
		sum[i]=0;
	}
	for(i=0;i<8;i++)
	{	
		*(adcdat+i)=ad7606_dat[i];
	}
}

/**
  * @}
  */

/**
  * @}
  */

/**********************************END OF FILE*****************************************/
