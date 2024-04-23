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

/*
AD603增益计算
Gain(dB)=40*V_gain+gain_K
*/

#include "AD603_P_VGA.h"
#include "TLC5615.h"

#include "stdint.h"
#include "math.h"

void AD603_Module_Init(AD603_M_HandleTypeDef *hAD603,TLC5615_HandleTypeDef *hTLC5615,float gain_K)
{
	/*指定AD603模块所使用的TLC5615*/
	hAD603->hTLC5615=hTLC5615;
	/*指定AD603模块对数增益常数项*/
	hAD603->gain_K=gain_K;
}

void AD603_Module_SetDigVoltage(AD603_M_HandleTypeDef *hAD603,float Vg)
{
	uint16_t Din;
	Din=(uint16_t)(1024*(Vg+0.75f)/1.5f);
	if(Din>1023)Din=1023;
	TLC5615_SetDigtalValue(hAD603->hTLC5615,Din);
}

/*设置模块分贝增益函数*/
void AD603_Module_SetDecibelGain(AD603_M_HandleTypeDef *hAD603,float DecGain)
{
	/*判断增益是否在输出线性范围内并进行约束*/
	if(DecGain<((hAD603->gain_K)-20))DecGain=(hAD603->gain_K)-20;
	else if(DecGain>((hAD603->gain_K)+20))DecGain=(hAD603->gain_K)+20;
	
	/*计算增益电压*/
	float Vgain=(DecGain-(hAD603->gain_K))/40;
	/*设置增益电压*/
	AD603_Module_SetDigVoltage(hAD603,Vgain);
}

/*设置模块线性增益函数*/
void AD603_Module_SetLinearGain(AD603_M_HandleTypeDef *hAD603,float Au)
{
	/*解分贝增益*/
	float DecGain=20*log10(Au);
	/*计算增益电压*/
	float Vgain=(DecGain-(hAD603->gain_K))/40;
	/*设置增益电压*/
	AD603_Module_SetDigVoltage(hAD603,Vgain);
}

/*计算模块分贝增益线性范围函数
*通过传入数组分别为下限和上限，增益常数gain_K最小为10
*/
void AD603_Module_CalcDecibelGainLinearRange(float *Range,float gain_K)
{
	if(gain_K<10)
	{
		*Range=-999;*(Range+1)=-999;
		return;
	}

	if(gain_K+20>60)
	{
		*(Range+1)=60;
	}
	else{*(Range+1)=gain_K+20;}

	if(gain_K-20<-10) *Range=-10;
	else *Range=gain_K-20;
	return;
}

/*计算模块线性最大增益函数
*返回值为最大增益，增益常数gain_K最小为10
*/
float AD603_Module_CalcMaxLineaGain(float gain_K)
{
  float range[2];
  AD603_Module_CalcDecibelGainLinearRange(range,gain_K);
	if(range[0]==range[1])return 0;
  else return pow(10,range[1]/20);
}
