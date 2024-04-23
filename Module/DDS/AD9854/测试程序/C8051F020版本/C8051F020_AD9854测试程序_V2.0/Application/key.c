//-----------------------------------------------------------------
// 程序描述:
// 　　独立键盘驱动程序
// 作　　者: 凌智电子
// 开始日期: 2014-02-1
// 完成日期: 2014-02-2
// 修改日期: 2014-02-2
// 当前版本: V1.0
// 历史版本:
// 　- V1.0: 独立键盘扫描,键值返回
// 调试工具: 凌智C8051F核心板V1.1、1602液晶屏
// 说　　明: 
//				
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// 头文件包含
//-----------------------------------------------------------------
#include "key.h"
#include "delay.h"
#include "type.h"

//-----------------------------------------------------------------
// uchar Key_Scan(void)
//-----------------------------------------------------------------
//
// 函数功能: 独立键盘扫描
// 入口参数: 无
// 返回参数: rst：键值
// 全局变量: 无
// 调用模块: 无    
// 注意事项: 无
//-----------------------------------------------------------------
uchar Key_Scan(void)
{
	uchar rst = 0;
	
	// 按键隐藏优先级为KEY1>KEY2>KEY3>KEY4
	if((KEY1==0) || (KEY2==0) || (KEY3==0) || (KEY4==0))
	{
		Delay_1ms(10);			// 延时10ms去抖动
		if(KEY1 == 0)				// KEY1按下
		{
			rst = 1;
			while(KEY1 == 0); // 松手检测
		}
		if(KEY2 == 0)
		{
			rst = 2;
			while(KEY2 == 0);
		}
		if(KEY3 == 0)
		{
			rst = 3;
			while(KEY3 == 0);
		}
		if(KEY4 == 0)
		{
			rst = 4;
			while(KEY4 == 0);
		}
	}
	else
		rst = 0;
	return rst;
}
//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------