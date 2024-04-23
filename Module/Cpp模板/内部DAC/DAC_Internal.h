#ifndef	_DAC_INTERNAL_H
#define _DAC_INTERNAL_H
extern "C"{
	#include "stm32f4xx_dac.h"
}
#define MIN_GAIN 			(-2)	// 可设置的最小增益
#define MAX_GAIN 			(40)	// 可设置的最大增益
#define AUTORELOADVALUE  	2000	// 设定
#define VREF  				3287

class	DAC_Internal
{
public:
	DAC_Internal();
	~DAC_Internal();
public:
	void AM_Change();
	void DAC_Reset(int AGAIN); 
private:
	void func_init();
};

#endif
