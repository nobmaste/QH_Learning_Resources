#ifndef INIT_H
#define INIT_H

extern void init(void);
extern void DelayUs2x(unsigned char t);
extern void DelayMs(unsigned char t);
extern bit LCD_Check_Busy(void);
extern void LCD_Write_Com(unsigned char com);
extern void LCD_Write_Data(unsigned char Data); 
extern void LCD_Clear(void);
extern void LCD_Write_String(unsigned char x,unsigned char y,unsigned char *s);
extern void LCD_Write_Char(unsigned char x,unsigned char y,unsigned char Data);
extern void LCD_Init(void);

#endif