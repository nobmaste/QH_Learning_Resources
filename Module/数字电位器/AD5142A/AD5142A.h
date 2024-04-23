#ifndef __AD5142_H
#define __AD5142_H

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_i2c.h"

#include "delay.h"

/*RADC_AddressBits*/
#define AD5142A_RADC1 0x00
#define AD5142A_RADC2 0x01

/*RADC_ReadAddressBits*/
#define AD5142A_ReadCH1 0x00
#define AD5142A_ReadCH2 0x02

/*EEPROM & RADC DataSource*/
#define AD5142A_DataSourece_EEPROM 0x01
#define AD5142A_DataSourece_RADC 0x03

/*开启扩展应用*/
#define __AD5142A_EXAPP_

typedef struct
{
	I2C_HandleTypeDef *hi2c;
	uint8_t I2C_Address;
	GPIO_InitTypeDef *nRST;
	GPIO_InitTypeDef *INDEP;
	float Max_Resistance;
}AD5142A_HandleTypeDef;

void AD5142A_Conn_Init(void);
void AD5142A_TransmitData(AD5142A_HandleTypeDef *hAD5142A,uint16_t dat);
uint8_t AD5142A_ReadData(AD5142A_HandleTypeDef *hAD5142A,uint8_t *datSource);
void AD5142A_Nop(AD5142A_HandleTypeDef *hAD5142A);
void AD5142A_WriteChannel(AD5142A_HandleTypeDef *hAD5142A,uint8_t RADC_Channel,uint8_t RADC_Value);
void AD5142A_WriteInputReg(AD5142A_HandleTypeDef *hAD5142A,uint8_t RADC_Channel,uint8_t RADC_Value);
uint8_t AD5142A_ReadBackValue(AD5142A_HandleTypeDef *hAD5142A,uint8_t ReadChannel,uint8_t DataSourece);
void AD5142A_SaveChannel(AD5142A_HandleTypeDef *hAD5142A,uint8_t RADC_Channel);
void AD5142A_SetSaveChannel(AD5142A_HandleTypeDef *hAD5142A,uint8_t RADC_Channel);
void AD5142A_sRST(AD5142A_HandleTypeDef *hAD5142A);
void AD5142A_sShutdown(AD5142A_HandleTypeDef *hAD5142A,uint8_t RADC_Channel,uint8_t ENABLE);
void AD5142A_SetTopScale(AD5142A_HandleTypeDef *hAD5142A,uint8_t RADC_Channel,uint8_t MODE);
void AD5142A_SetBottomScale(AD5142A_HandleTypeDef *hAD5142A,uint8_t RADC_Channel,uint8_t MODE);

#ifdef __AD5142A_EXAPP_
void AD5142A_SetResistance(AD5142A_HandleTypeDef *hAD5142A,uint8_t RADC_Channel,float Resistance);
void AD5142A_SetResDivider(AD5142A_HandleTypeDef *hAD5142A,uint8_t RADC_Channel,float kDivide);
#endif

#endif
