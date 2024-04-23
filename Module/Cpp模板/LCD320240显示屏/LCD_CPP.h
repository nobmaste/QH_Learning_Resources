#ifndef	__LCD_CPP
#define __LCD_CPP

extern "C" { 
#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "lcd.h"
#include "stm32f4xx_fsmc.h"
}
//用于设置LCD的横屏与竖屏默认为1：横屏模式
#define HOR 1
#define VIR 0

class LCD_Config
{
	
private:
	int font_color;			//字体颜色
	int background_color;	//背景色
	char screen;			//屏幕（横屏或竖屏）

public:
	//默认初始化接口
	LCD_Config();
	//自定义初始化接口
	LCD_Config(int font_color,int background_color,char screen);
	~LCD_Config();

	//状态栏
	void status_bar(int color,char screen = 0);

private:
	//LCD初始化函数
	void LCD_Initlize(int font_color,int background_color,char screen);
};

#endif /*__LCD_CPP*/
