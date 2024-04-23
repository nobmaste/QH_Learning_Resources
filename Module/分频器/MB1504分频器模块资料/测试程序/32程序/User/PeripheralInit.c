//-----------------------------------------------------------------
// 程序描述:
// 　	 MB1504分频器分频测试程序
// 作　　者: 凌智电子
// 开始日期: 2017-01-28
// 完成日期: 2017-01-28
// 修改日期:  
// 当前版本: V1.0
// 历史版本:
// 　- V1.0: 
// 调试工具: 凌智STM32+FPGA电子系统设计开发板、LZE_ST_LINK2
// 说　　明: 
// 测试功能: 
//		 引脚连接: 	单片机    MB1504模块
//                PA4-->CLK		PA5-->DATA		PA7-->LE		
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// 头文件包含==-----------------------------
#include <stm32f10x.h>
#include "Delay.h"
#include "MB1504.h"
#include "PeripheralInit.h"

//-----------------------------------------------------------------
// 初始化程序区
//-----------------------------------------------------------------
//-----------------------------------------------------------------
// void PeripheralInit(void)
//-----------------------------------------------------------------
// 
// 函数功能: 系统外设初始化
// 入口参数: 无 
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void PeripheralInit(void)
{
	GPIO_MB1504_Configuration();									// MB1504 GPIO配置
}

//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
