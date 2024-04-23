//-----------------------------------------------------------------
// HD44780(KS0066U)内核的字符型LCD子程序的头文件
// 头文件名:CharLCD.h
// 作    者:凌智电子
// 编写时间:2009.11.23
// 修改日期:
//-----------------------------------------------------------------

#ifndef _CHARLCD_H
#define _CHARLCD_H

//-----------------------------------------------------------------
// 数据类型宏定义
//-----------------------------------------------------------------
#define uchar unsigned char
#define uint  unsigned int

//-----------------------------------------------------------------
// I/O口定义:LCM控制/数据线接口
//-----------------------------------------------------------------
#define RS_set (P5 |= 0x02)           // 0000 0010
#define RW_set (P5 |= 0x04)           // 0000 0100
#define E_set	 (P5 |= 0x08)           // 0000 1000

#define RS_clr (P5 &= 0xfd)           // 1111 1101
#define RW_clr (P5 &= 0xfb)           // 1111 1011
#define E_clr	 (P5 &= 0xf7)           // 1111 0111

#define DLCD    P5							// 数据端口在P5口的高四位

//-----------------------------------------------------------------
// 外部变量声明
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// 外部函数声明
//-----------------------------------------------------------------
extern void LCM_Init (void);									// LCM初始化,使用时根据需要选择
extern void WrCLcdC (uchar lcdcomm);					// 写LCM的指令代码
extern void WrCLcdD (uchar lcddata);					// 写LCM要显示的数据
extern void WriteString (uchar x, uchar y, uchar *s);// 指定位置显示一串字符串
extern void CG_Write (void);           				// 将自定义的字符送到CGRAM
extern void Wr_CodeData (void);            		// 写操作共同部分程序
extern void WrCLcd_char_num (uchar x, uchar y, uchar uchar_num);// 显示十进制无符号字符数据
extern void WrCLcd_int_num (uchar x, uchar y, uint  uint_num);	// 显示十进制无符号整形数据
extern void WrCLcd_long_num (uchar x, uchar y, unsigned long ulong_num);// 显示十进制无符号长整形数据

#endif

//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
