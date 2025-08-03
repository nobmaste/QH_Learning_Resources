
#define AD5933_CTRL_NOP					(0)<<12
#define AD5933_SYS_Init					(1)<<12
#define AD5933_Begin_Fre_Scan		    (2)<<12
#define AD5933_Fre_UP					(3)<<12
#define AD5933_Fre_Rep					(4)<<12
#define AD5933_Get_Temp					(9)<<12
#define AD5933_Sleep					(10)<<12
#define AD5933_Standby					(11)<<12

#define AD5933_OUTPUT_2V				(0)<<9
#define AD5933_OUTPUT_1V				(3)<<9
#define AD5933_OUTPUT_400mV			    (2)<<9
#define AD5933_OUTPUT_200mV			    (1)<<9

#define AD5933_Gain_1					(1)<<8//增益
#define AD5933_Gain_5				  	(0)<<8

#define AD5933_IN_MCLK					(0)<<3//内部时钟
#define AD5933_OUT_MCLK					(1)<<3//外部时钟

#define AD5933_Reset			  		(1)<<4
#define AD5933_NReset				    (0)<<4

void Init_AD5933(void);
void Fre_To_Hex(float fre, u8 *buf);
void AD5933_Set_Freq_Start( unsigned int freq );
void AD5933_Set_Freq_Add( unsigned int afreq );
void AD5933_Set_Freq_Num( unsigned int num );
void AD5933_Set_Mode( unsigned int ctrl, unsigned int out, unsigned int gain, unsigned int clk, unsigned int rst );
void AD5933_Set_Mode_Rst(void);
void AD5933_Set_Mode_Standby(void);
void AD5933_Set_Mode_SysInit(void);
void AD5933_Set_Mode_Freq_Start(void);
void AD5933_Set_Mode_Freq_UP(void);
void AD5933_Set_Mode_Freq_Repeat(void);
void AD5933_Set_Mode_Freq_Temp(void);
unsigned char  AD5933_Get_DFT_ST(void);
unsigned int AD5933_Get_Real(void);
unsigned int AD5933_Get_Img(void);
