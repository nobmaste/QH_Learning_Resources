#ifndef __AD7606_H_
#define __AD7606_H_

/*7606 RANGE 高电平10V量程 低电平5V量程*/
#define RANGE 10

//使用硬件SPI
#define __STM32_HARDWARE_SPI_

void AD7606_Init(void);
void AD7606_Reset(void);
void AD7606_StartConv(void);
void AD7606_ReadConvData(void);
#endif
