/*
 **********************************************************************
 *Copyright(c)		Lance Zhang
 *Identification	CHIC
 *Describtion		Template
 *
 *Present Version	1.0
 *Writer			Lance Zhang
 *Date				8/16/2018
 **********************************************************************
 *W25Q16系列共有32个block，每个block 64KB，每个block包括16个sector，
 *每个sector 4KB.每个block包括256个page。
 **********************************************************************
 *@Attention:
 *在执行擦除扇区操作时，需要对输入的字节数除以4096
 *原因：
 *写入函数：W25Q16_FLASH::W25QXX_Write()中输入的地址是以字节为单位的，
 *除以4096(即4KB)得到扇区的编号。
 *以BLOCK 0为例，最后一个扇区的编号为15，
 *则在写入时输入的地址为：15*4096 = F000(h)即扇区15的起始地址
 *擦除函数：W25QXX_Erase_Sector()中输入的是扇区的编号，编号*4096 = 字节数
 **********************************************************************
**/
#ifndef	_W25Q16_H
#define _W25Q16_H

extern "C"
{
	#include "stm32f4xx_gpio.h"
	#include "delay.h"
}
#include "SPI.h"

/*W25X系列/Q系列芯片列表	   
W25Q80  ID  0XEF13
W25Q16  ID  0XEF14
W25Q32  ID  0XEF15
W25Q64  ID  0XEF16	
W25Q128 ID  0XEF17*/	
#define W25Q80 	0XEF13 	
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16
#define W25Q128	0XEF17
//定义W25QXX芯片型号
extern u16 W25QXX_TYPE;							   
//W25QXX的片选信号
#define	W25QXX_CS 		PBout(0)  		

////////////////////////////////////////////////////////////////////////////////// 
//指令表见手册第19页
//Byte 0：
#define W25X_WriteEnable		0x06 
#define W25X_WriteDisable		0x04 
#define W25X_ReadStatusReg		0x05 
#define W25X_WriteStatusReg		0x01 
#define W25X_ReadData			0x03 
#define W25X_FastReadData		0x0B 
#define W25X_FastReadDual		0x3B 
#define W25X_PageProgram		0x02 
#define W25X_BlockErase			0xD8 
#define W25X_SectorErase		0x20 
#define W25X_ChipErase			0xC7 
#define W25X_PowerDown			0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID			0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F 

#define W25Q16_SIZE	2*1024*1024

class	W25Q16_FLASH : public SPI
{
public:
	W25Q16_FLASH();
	~W25Q16_FLASH();

private:
	u16 	W25QXX_TYPE;	//默认是W25Q16
public:
	u32 	FLASH_SIZE;		//FLASH大小
	//sector_id * 4096 = FLASH_SIZE
	u32		SECTOR_ID(u8 sector_id);

public:
	void W25QXX_Init(void);
	u16  W25QXX_ReadID(void);  	    											//读取FLASH ID
	u8	 W25QXX_ReadSR(void);        											//读取状态寄存器 
	void W25QXX_Write_SR(u8 sr);  												//写状态寄存器
	void W25QXX_Write_Enable(void);  											//写使能 
	void W25QXX_Write_Disable(void);											//写保护
	void W25QXX_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
	void W25QXX_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);   			//读取flash
	void W25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);			//写入flash
	void W25QXX_Erase_Chip(void);    	  										//整片擦除
	void W25QXX_Erase_Sector(u32 Dst_Addr);										//扇区擦除
	void W25QXX_Wait_Busy(void);           										//等待空闲
	void W25QXX_PowerDown(void);        										//进入掉电模式
	void W25QXX_WAKEUP(void);													//唤醒
	void W25QXX_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
};

#endif
