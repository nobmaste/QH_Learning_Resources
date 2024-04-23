#ifndef INIT_H
#define INIT_H

extern void init();
extern void LCD_Init();
void DelayUs2x(unsigned char t);
void DelayMs(unsigned char t);
bit LCD_Check_Busy(void);
void LCD_Write_Com(unsigned char com);
void LCD_Write_Data(unsigned char Data); 
void LCD_Clear(void);
void LCD_Write_String(unsigned char x,unsigned char y,unsigned char *s);
void LCD_Write_Char(unsigned char x,unsigned char y,unsigned char Data);
void LCD_Init(void);
#endif