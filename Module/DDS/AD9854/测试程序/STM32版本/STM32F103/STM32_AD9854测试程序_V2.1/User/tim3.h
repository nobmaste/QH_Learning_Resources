#ifndef _tim3_H
#define _tim3_H

extern u16 TIM_FLAG;
extern void VVInit(void);
extern void NVIC_Configuration(void);
extern void tim_set (uint32_t fre_out, uint32_t tim_clk);

extern void TIM3_Init(void);

#endif

