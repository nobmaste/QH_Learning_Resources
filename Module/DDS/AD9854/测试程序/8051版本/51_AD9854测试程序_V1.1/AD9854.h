#ifndef AD9854_H
#define AD9854_H

#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long

void AD9854_WR_Byte(uchar Adata);
void AD9854_Init(void);
void Freq_convert(long Freq);   
void AD9854_SetSine(ulong Freq,uint Shape);

#endif