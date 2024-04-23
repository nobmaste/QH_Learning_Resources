#include "IWDG.h"

watch_dog::watch_dog()
{
	
}

watch_dog::~watch_dog()
{
	
}

void watch_dog::IWDG_Init(u8 prer, u16 rlr)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(prer);
	IWDG_SetReload(rlr);
	IWDG_ReloadCounter();
	IWDG_Enable();
}

void watch_dog::feed()
{
	IWDG_ReloadCounter();	
}
