#ifndef __TLC5615_H
#define __TLC5615_H

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_spi.h"

#include "delay.h"

typedef struct
{
	SPI_HandleTypeDef *hspi;
	GPIO_InitTypeDef CS_GPIO;
	GPIO_TypeDef* GPIO_GROUP;
	float Vref;
}TLC5615_HandleTypeDef;

void TLC5615_Conn_Init(void);
void TLC5615_TransmitData(TLC5615_HandleTypeDef *hTLC5615,uint16_t dat);
void TLC5615_SetDigtalValue(TLC5615_HandleTypeDef *hTLC5615,uint16_t dat);
void TLC5615_SetOutputVoltage(TLC5615_HandleTypeDef *hTLC5615,float voltage);

//#ifdef __AD5142A_EXAPP_
//void AD5142A_SetResistance(AD5142A_HandleTypeDef *hAD5142A,uint8_t RADC_Channel,float Resistance);
//void AD5142A_SetResDivider(AD5142A_HandleTypeDef *hAD5142A,uint8_t RADC_Channel,float kDivide);
//#endif

#endif
