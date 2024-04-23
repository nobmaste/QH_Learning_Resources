#ifndef _CAPTURE_H
#define _CAPTURE_H

extern "C"
{
#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "stm32f4xx_tim.h"
}

class Capture
{
public:
	Capture();
	~Capture();
private:
	void Capture_Init(u32 arr,u16 psc);
public:
	u32 arr;
	u16 psc;
};

#endif
