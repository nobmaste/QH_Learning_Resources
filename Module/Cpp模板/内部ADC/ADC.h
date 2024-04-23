#ifndef __ADC_H
#define __ADC_H
extern "C"
{
	#include "stm32f4xx_adc.h"
}
#include "COUNTER.h"
//extern u16 VOLTAGE;

class ADC_Internal : COUNTER
{
public:
	ADC_Internal(bool interrupt_used = true);
	~ADC_Internal();

	u16 Get_Adc(u8 ch = ADC_Channel_5);
	u16 Get_Adc_Average(u8 ch,u8 times);

	u16 *vol;
private:
	void func_Init();
};

#endif
