/*
AD603程控-压控放大器_驱动程序
Version 1.00

本程序是wrx设计并制作的AD603压控放大器模块的驱动程序；
通过驱动DAC芯片TLC5615,实现0-4V直流电压输出，并通过运算放大电路实现±750mV增益控制电压输出，以实现对增益的程序控制。

本程序依赖于TLC5615驱动程序！

增益量程切换为手动控制。

***本程序仅供2021年全国大学生电子设计竞赛本组使用，绝不外传***

(C)LWNF 2015-2021
*/
#ifndef __AD603_P_VGA_H
#define __AD603_P_VGA_H

#include "TLC5615.h"
#include "stdint.h"

typedef struct
{
	TLC5615_HandleTypeDef *hTLC5615;
	float gain_K;
}AD603_M_HandleTypeDef;

void AD603_Module_Init(AD603_M_HandleTypeDef *hAD603,TLC5615_HandleTypeDef *hTLC5615,float gain_K);
void AD603_Module_SetDigVoltage(AD603_M_HandleTypeDef *hAD603,float Vg);
void AD603_Module_SetDecibelGain(AD603_M_HandleTypeDef *hAD603,float DecGain);
void AD603_Module_SetLinearGain(AD603_M_HandleTypeDef *hAD603,float Au);
void AD603_Module_CalcDecibelGainLinearRange(float *Range,float gain_K);
float AD603_Module_CalcMaxLineaGain(float gain_K);

#endif
