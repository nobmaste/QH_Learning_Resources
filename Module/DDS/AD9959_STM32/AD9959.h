#ifndef _AD9959_H_
#define _AD9959_H_
#include "stm32f4xx_hal.h"
#include "stdint.h"


//AD9959管脚宏定义
#define CS_HIGH()        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_SET)
#define CS_LOW()         HAL_GPIO_WritePin(GPIOD, GPIO_PIN_11, GPIO_PIN_RESET)

#define SCLK_HIGH()      HAL_GPIO_WritePin(GPIOG, GPIO_PIN_5, GPIO_PIN_SET)
#define SCLK_LOW()       HAL_GPIO_WritePin(GPIOG, GPIO_PIN_5, GPIO_PIN_RESET)

#define UPDATE_HIGH()    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_10, GPIO_PIN_SET)
#define UPDATE_LOW()     HAL_GPIO_WritePin(GPIOG, GPIO_PIN_10, GPIO_PIN_RESET)

// 并口选择
#define PS0_HIGH()       HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET)
#define PS0_LOW()        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET)

#define PS1_HIGH()       HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_SET)
#define PS1_LOW()        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_0, GPIO_PIN_RESET)

#define PS2_HIGH()       HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_SET)
#define PS2_LOW()        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_RESET)

#define PS3_HIGH()       HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, GPIO_PIN_SET)
#define PS3_LOW()        HAL_GPIO_WritePin(GPIOD, GPIO_PIN_5, GPIO_PIN_RESET)

// 并口数据线
#define SDIO0_HIGH()     HAL_GPIO_WritePin(GPIOG, GPIO_PIN_2, GPIO_PIN_SET)
#define SDIO0_LOW()      HAL_GPIO_WritePin(GPIOG, GPIO_PIN_2, GPIO_PIN_RESET)

#define SDIO1_HIGH()     HAL_GPIO_WritePin(GPIOG, GPIO_PIN_4, GPIO_PIN_SET)
#define SDIO1_LOW()      HAL_GPIO_WritePin(GPIOG, GPIO_PIN_4, GPIO_PIN_RESET)

#define SDIO2_HIGH()     HAL_GPIO_WritePin(GPIOG, GPIO_PIN_3, GPIO_PIN_SET)
#define SDIO2_LOW()      HAL_GPIO_WritePin(GPIOG, GPIO_PIN_3, GPIO_PIN_RESET)

#define SDIO3_HIGH()     HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET)
#define SDIO3_LOW()      HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET)

// 其他控制信号
#define AD9959_PWR_HIGH() HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET)
#define AD9959_PWR_LOW()  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET)

#define RESET_HIGH()     HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_SET)
#define RESET_LOW()      HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET)



//AD9959寄存器地址定义
#define CSR_ADD   0x00   //CSR 通道选择寄存器
#define FR1_ADD   0x01   //FR1 功能寄存器1
#define FR2_ADD   0x02   //FR2 功能寄存器2
#define CFR_ADD   0x03   //CFR 通道功能寄存器

#define CFTW0_ADD 0x04   //CTW0 通道频率转换字寄存器
#define CPOW0_ADD 0x05   //CPW0 通道相位转换字寄存器
#define ACR_ADD   0x06   //ACR 幅度控制寄存器

#define LSRR_ADD  0x07   //LSR 线性扫描斜率寄存器
#define RDW_ADD   0x08   //RDW 上升扫描增量寄存器
#define FDW_ADD   0x09   //FDW 下降扫描增量寄存器

#define PROFILE_ADDR_BASE   0x0A   //Profile寄存器,配置文件寄存器起始地址

//CSR[7:4]通道选择启用位
#define CH0 0x10
#define CH1 0x20
#define CH2 0x40
#define CH3 0x80

//FR1[9:8] 调制电平选择位
#define LEVEL_MOD_2  	0x00//2电平调制 2阶调制
#define LEVEL_MOD_4		0x01//4电平调制	4阶调制
#define LEVEL_MOD_8		0x02//8电平调制	8阶调制
#define LEVEL_MOD_16	0x03//16电平调制	16阶调制

//CFR[23:22]  幅频相位（AFP）选择位
#define	DISABLE_Mod		0x00	//00	调制已禁用
#define	ASK 					0x40	//01	振幅调制，幅移键控
#define	FSK 					0x80	//10	频率调制，频移键控
#define	PSK 					0xc0	//11	相位调制，相移键控

//（CFR[14]）线性扫描启用 sweep enable																				
#define	SWEEP_ENABLE	0x40	//1	启用
#define	SWEEP_DISABLE	0x00	//0	不启用
		
void delay1 (uint32_t length);//延时
void IntReset(void);	 			//AD9959复位
void IO_Update(void); 		  //AD9959更新数据
void Intserve(void);				//IO口电平状态初始化
void AD9959_Init(void);			//IO口初始化

/***********************AD9959基本寄存器操作函数*****************************************/
void AD9959_WriteData(uint8_t RegisterAddress, uint8_t NumberofRegisters, uint8_t *RegisterData);//向AD9959写数据
void Write_CFTW0(uint32_t fre);										//写CFTW0通道频率转换字寄存器
void Write_ACR(uint16_t Ampli);										//写ACR通道幅度转换字寄存器
void Write_CPOW0(uint16_t Phase);									//写CPOW0通道相位转换字寄存器

void Write_LSRR(uint8_t rsrr,uint8_t fsrr);				//写LSRR线性扫描斜率寄存器
void Write_RDW(uint32_t r_delta);									//写RDW上升增量寄存器
void Write_FDW(uint32_t f_delta);									//写FDW下降增量寄存器

void Write_Profile_Fre(uint8_t profile,uint32_t data);//写Profile寄存器,频率
void Write_Profile_Ampli(uint8_t profile,uint16_t data);//写Profile寄存器,幅度
void Write_Profile_Phase(uint8_t profile,uint16_t data);//写Profile寄存器,相位
/********************************************************************************************/


/*****************************点频操作函数***********************************/
void AD9959_Set_Fre(uint8_t Channel,uint32_t Freq); //写频率
void AD9959_Set_Amp(uint8_t Channel, uint16_t Ampli);//写幅度
void AD9959_Set_Phase(uint8_t Channel,uint16_t Phase);//写相位
/****************************************************************************/

/*****************************调制操作函数  ***********************************/
void AD9959_Modulation_Init(uint8_t Channel,uint8_t Modulation,uint8_t Sweep_en,uint8_t Nlevel);//设置各个通道的调制模式。
void AD9959_SetFSK(uint8_t Channel, uint32_t *data,uint16_t Phase);//设置FSK调制的参数
void AD9959_SetASK(uint8_t Channel, uint16_t *data,uint32_t fre,uint16_t Phase);//设置ASK调制的参数
void AD9959_SetPSK(uint8_t Channel, uint16_t *data,uint32_t Freq);//设置PSK调制的参数

void AD9959_SetFre_Sweep(uint8_t Channel, uint32_t s_data,uint32_t e_data,uint32_t r_delta,uint32_t f_delta,uint8_t rsrr,uint8_t fsrr,uint16_t Ampli,uint16_t Phase);//设置线性扫频的参数
void AD9959_SetAmp_Sweep(uint8_t Channel, uint32_t s_Ampli,uint16_t e_Ampli,uint32_t r_delta,uint32_t f_delta,uint8_t rsrr,uint8_t fsrr,uint32_t fre,uint16_t Phase);//设置线性扫幅的参数
void AD9959_SetPhase_Sweep(uint8_t Channel, uint16_t s_data,uint16_t e_data,uint16_t r_delta,uint16_t f_delta,uint8_t rsrr,uint8_t fsrr,uint32_t fre,uint16_t Ampli);//设置线性扫相的参数
/********************************************************************************************/

#endif








