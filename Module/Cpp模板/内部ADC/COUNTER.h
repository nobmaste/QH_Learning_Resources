#ifndef __COUNTER_H
#define __COUNTER_H
extern "C"
{
	#include "stm32f4xx_tim.h"
	#include "sys.h"
	#include "interrupt.h"
}

class COUNTER
{
public:
	COUNTER();
	~COUNTER();
	void TIM3_Int_Init(u16 arr = 20-1,u16 psc = 84-1);
private:
	void LED_Init(void);
};

#endif
