#ifndef __AD9959_H__
#define __AD9959_H__

#include "stm32f4xx.h"



#define AD9959_CS_RCC_GPIO      RCC_AHB1Periph_GPIOA
#define AD9959_SCLK_RCC_GPIO    RCC_AHB1Periph_GPIOA
#define AD9959_SDIO0_RCC_GPIO   RCC_AHB1Periph_GPIOA
#define AD9959_SDIO1_RCC_GPIO   RCC_AHB1Periph_GPIOA
#define AD9959_SDIO2_RCC_GPIO   RCC_AHB1Periph_GPIOA
#define AD9959_SDIO3_RCC_GPIO   RCC_AHB1Periph_GPIOA
#define AD9959_UPDATE_RCC_GPIO  RCC_AHB1Periph_GPIOA
#define AD9959_PS0_RCC_GPIO     RCC_AHB1Periph_GPIOA
#define AD9959_PS1_RCC_GPIO     RCC_AHB1Periph_GPIOA
#define AD9959_PS2_RCC_GPIO     RCC_AHB1Periph_GPIOA
#define AD9959_PS3_RCC_GPIO     RCC_AHB1Periph_GPIOA
#define AD9959_RST_RCC_GPIO     RCC_AHB1Periph_GPIOA
#define AD9959_PWD_RCC_GPIO     RCC_AHB1Periph_GPIOA

#define AD9959_CS_GPIO      GPIOA
#define AD9959_SCLK_GPIO    GPIOA
#define AD9959_SDIO0_GPIO   GPIOA
#define AD9959_SDIO1_GPIO   GPIOA
#define AD9959_SDIO2_GPIO   GPIOA
#define AD9959_SDIO3_GPIO   GPIOA
#define AD9959_UPDATE_GPIO  GPIOA
#define AD9959_PS0_GPIO     GPIOA
#define AD9959_PS1_GPIO     GPIOA
#define AD9959_PS2_GPIO     GPIOA
#define AD9959_PS3_GPIO     GPIOA
#define AD9959_RST_GPIO     GPIOA
#define AD9959_PWD_GPIO     GPIOA

#define AD9959_CS_Pin      GPIO_Pin_0
#define AD9959_SCLK_Pin    GPIO_Pin_1
#define AD9959_SDIO0_Pin   GPIO_Pin_2
#define AD9959_SDIO1_Pin   GPIO_Pin_0
#define AD9959_SDIO2_Pin   GPIO_Pin_0
#define AD9959_SDIO3_Pin   GPIO_Pin_0
#define AD9959_UPDATE_Pin  GPIO_Pin_3
#define AD9959_PS0_Pin     GPIO_Pin_0
#define AD9959_PS1_Pin     GPIO_Pin_0
#define AD9959_PS2_Pin     GPIO_Pin_0
#define AD9959_PS3_Pin     GPIO_Pin_0
#define AD9959_RST_Pin     GPIO_Pin_4
#define AD9959_PWD_Pin     GPIO_Pin_5

#define AD9959_CSR_ADD 0x00
#define AD9959_FR1_ADD 0x01
#define AD9959_FR2_ADD 0x02
#define AD9959_CFR_ADD 0x03
#define AD9959_CFTW0_ADD 0x04
#define AD9959_CPOW0_ADD 0x05
#define AD9959_ACR_ADD 0x06
#define AD9959_LSRR_ADD 0x07
#define AD9959_RDW_ADD 0x08
#define AD9959_FDW_ADD 0x09




void AD9959_UpDate(void);
void AD9959_WriteByte(unsigned char NewByte);
extern void AD9959_Init(void);

extern void AD9959_WriteFreq(unsigned char CH, double Freq);
extern void AD9959_WriteAmplitude(unsigned char CH, unsigned int Amplitude);
extern void AD9959_Write_Phase(unsigned char CH, unsigned int Phase);

#endif

