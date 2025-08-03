
#include "main.h"

#define TRI_WAVE 	0  		//输出三角波
#define SIN_WAVE 	1		//输出正弦波
#define SQU_WAVE 	2		//输出方波


void AD9833_WaveSeting(double frequence,unsigned int frequence_SFR,unsigned int WaveMode,unsigned int Phase );
void AD9833_Write(unsigned int TxData);
void AD9833_Init_GPIO(void);
void AD9833_AmpSet(unsigned char amp);






