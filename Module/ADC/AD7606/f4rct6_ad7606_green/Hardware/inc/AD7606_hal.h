/**
  **************************************************************************************
  * @file AD7606_hal.H
  * @author shaokairu
  * @date 2021.8.2
  * @brief This file contains all the functions prototypes for 7606 board.
  **************************************************************************************
  * @note SERIAL_INTERFACE      ser--1 DB15--0
  *       PARALLEL_INTERFACE    ser--0 DB15--0
  *       PARALLEL_BYTE         ser--1 DB15--1
  **************************************************************************************
  */
  
/* Define to prevent recursive inclusion ---------------------------------------------*/
#ifndef __AD7606_H
#define __AD7606_H

/* Includes --------------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_gpio.h"

/* exported marco --------------------------------------------------------------------*/
#define SERIAL_INTERFACE 1
#define PARALLEL_INTERFACE 0
#define PARALLEL_BYTE 0

/* range -----------------------------------------------------------------------------*/
#define range 5

/** @defgroup gpio define
  * @{
  */
  
#ifdef SERIAL_INTERFACE

#define STBY_GPIO   GPIOA
#define STBY_PIN    GPIO_PIN_1
#define OS10_GPIO   GPIOA
#define OS10_PIN    GPIO_PIN_4
#define OS11_GPIO   GPIOB
#define OS11_PIN    GPIO_PIN_0
#define OS12_GPIO   GPIOB
#define OS12_PIN    GPIO_PIN_12
#define CONA_GPIO   GPIOC
#define CONA_PIN    GPIO_PIN_8
#define CONB_GPIO   GPIOC
#define CONB_PIN    GPIO_PIN_9
#define RST_GPIO    GPIOC
#define RST_PIN     GPIO_PIN_10
#define CLK_GPIO    GPIOC       // RD/SCLK
#define CLK_PIN     GPIO_PIN_11
#define CS_GPIO     GPIOC
#define CS_PIN      GPIO_PIN_12
#define FDA_GPIO    GPIOC       // FRD
#define FDA_PIN     GPIO_PIN_13
#define BUSY_GPIO   GPIOC
#define BUSY_PIN    GPIO_PIN_0
#define DAT_GPIO    GPIOC       // DB7
#define DAT_PIN     GPIO_PIN_1

#define STby_Set        HAL_GPIO_WritePin( STBY_GPIO, STBY_PIN, GPIO_PIN_SET)
#define OS10_Set        HAL_GPIO_WritePin( OS10_GPIO, OS10_PIN, GPIO_PIN_SET)
#define OS10_Reset      HAL_GPIO_WritePin( OS10_GPIO, OS10_PIN, GPIO_PIN_RESET)
#define OS11_Set 	    HAL_GPIO_WritePin( OS11_GPIO, OS11_PIN, GPIO_PIN_SET)
#define OS11_Reset      HAL_GPIO_WritePin( OS11_GPIO, OS11_PIN, GPIO_PIN_RESET)
#define OS12_Set 	    HAL_GPIO_WritePin( OS12_GPIO, OS12_PIN, GPIO_PIN_SET)
#define OS12_Reset      HAL_GPIO_WritePin( OS12_GPIO, OS12_PIN, GPIO_PIN_RESET)
#define convstA_Set 	HAL_GPIO_WritePin( CONA_GPIO, CONA_PIN, GPIO_PIN_SET)
#define convstA_Reset   HAL_GPIO_WritePin( CONA_GPIO, CONA_PIN, GPIO_PIN_RESET)
#define convstB_Set 	HAL_GPIO_WritePin( CONB_GPIO, CONB_PIN, GPIO_PIN_SET)
#define convstB_Reset   HAL_GPIO_WritePin( CONB_GPIO, CONB_PIN, GPIO_PIN_RESET)
#define rst_Set 	    HAL_GPIO_WritePin( RST_GPIO, RST_PIN, GPIO_PIN_SET)
#define rst_Reset  		HAL_GPIO_WritePin( RST_GPIO, RST_PIN, GPIO_PIN_RESET)
#define clk_Set 	    HAL_GPIO_WritePin( CLK_GPIO, CLK_PIN, GPIO_PIN_SET)
#define clk_Reset   	HAL_GPIO_WritePin( CLK_GPIO, CLK_PIN, GPIO_PIN_RESET)
#define cs_Set 	  		HAL_GPIO_WritePin( CS_GPIO, CS_PIN, GPIO_PIN_SET)
#define cs_Reset   		HAL_GPIO_WritePin( CS_GPIO, CS_PIN, GPIO_PIN_RESET)
#define frstdata_Set 	HAL_GPIO_WritePin( FDA_GPIO, FDA_PIN, GPIO_PIN_SET)
#define frstdata_Reset  HAL_GPIO_WritePin( FDA_GPIO, FDA_PIN, GPIO_PIN_RESET)
#define busy_Set 	    HAL_GPIO_WritePin( BUSY_GPIO, BUSY_PIN, GPIO_PIN_SET)
#define busy_Reset   	HAL_GPIO_WritePin( BUSY_GPIO, BUSY_PIN, GPIO_PIN_RESET)

#endif

/**
  * @}
  */

/* Exported functions prototypes -----------------------------------------------------*/
void AD7606_serial_GPIO_configuration(void);
void AD7606_reset(void);
void AD7606_startconvst(void);
void AD7606_init(void);
void AD7606_serial_read_data(int16_t * DB_data);
void AD7606_GetAnalogValue(int16_t *data,float *analog_data);
void adcgetvalue(int16_t *adcdat,uint8_t times);

#endif

/**********************************END OF FILE*****************************************/
