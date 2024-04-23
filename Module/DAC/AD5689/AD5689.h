#ifndef __AD5689_H
#define __AD5689_H

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_spi.h"

#include "delay.h"

typedef struct
{
	SPI_HandleTypeDef *hspi;
	GPIO_InitTypeDef SYNC_GPIO;
	GPIO_TypeDef* SYNC_GPIO_GROUP;
	GPIO_InitTypeDef LADC_GPIO;
	GPIO_TypeDef* LADC_GPIO_GROUP;
	GPIO_InitTypeDef RESET_GPIO;
	GPIO_TypeDef* RESET_GPIO_GROUP;
}AD5689_HandleTypeDef;

/*DAC通道选择*/
#define     DAC_A    								0x01
#define     DAC_B										0x08
#define     DAC_ALL									0x09

/*LADC屏蔽操作*/
#define     LDAC_Enable     				0
#define     LDAC_Disable						1

/*菊花链使能(DCEN)寄存器*/
#define     Standalone_mode 				0
#define     DCEN_mode								1 

/*命令操作表*/
#define CMD_WRITEn            	    1
#define CMD_WRITEn_UPDATEn          2
#define CMD_WRITE_UPDATEn           3
#define CMD_POWER_DOWN              4
#define CMD_LDAC_MASK               5
#define CMD_RESET               		6
#define CMD_DCEN_REG            		8
#define CMD_READ_BACK          		  9

void AD5689_Conn_Init(void);
void AD5689_Init(AD5689_HandleTypeDef *hAD5689);
void AD5689_SetRegisterValue(AD5689_HandleTypeDef *hAD5689,uint8_t command,uint16_t channel,uint16_t data);
void AD5689_LDAC_MASK(AD5689_HandleTypeDef *hAD5689,uint16_t ldac1,uint16_t ldac2);
void AD5689_Reset(AD5689_HandleTypeDef *hAD5689);
void AD5689_PowerDown(AD5689_HandleTypeDef *hAD5689,uint16_t powerdown_channel1,uint16_t powerdown_channel2);
void AD5689_WriteUpdate_DACREG(AD5689_HandleTypeDef *hAD5689,uint16_t channel,uint16_t data);
//void AD5689_Internal_Reference(AD5689_HandleTypeDef *hAD5689,uint16_t ref);
void AD5689_Write_InputREG(AD5689_HandleTypeDef *hAD5689,uint16_t channel,uint16_t data);
void AD5689_InputREG_toUpdate_DACREG(AD5689_HandleTypeDef *hAD5689,uint16_t channel,uint16_t data);
void AD5689_DAISYCHAIN_OPERATION(AD5689_HandleTypeDef *hAD5689,uint16_t decn);

#endif

