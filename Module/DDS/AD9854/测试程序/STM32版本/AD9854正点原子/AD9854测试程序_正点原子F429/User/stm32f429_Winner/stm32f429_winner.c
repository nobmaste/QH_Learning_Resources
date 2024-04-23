//-----------------------------------------------------------------
// 程序描述:
//     WinnerI开发板常用功能定义
// 作    者: 凌智电子
// 开始日期: 2018-08-04
// 完成日期: 2018-08-04
// 修改日期: 
// 当前版本: V1.0
// 历史版本:
//  - V1.0: (2018-08-04)提供一些固件功能，用于管理STM32.
// 调试工具: 凌智STM32F429+CycloneIV电子系统设计开发板、LZE_ST_LINK2
// 说    明: 
//    
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// 头文件包含
//-----------------------------------------------------------------
#include "stm32f429_winner.h"
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// void SystemClock_Config(u32 plln,u32 pllm,u32 pllp,u32 pllq)
//-----------------------------------------------------------------
// 
// 函数功能: 系统时钟初始化
// 入口参数: u32 plln: 主PLL倍频系数(PLL倍频),取值范围:64~432.
//					 u32 pllm: 主PLL和音频PLL分频系数(PLL之前的分频),取值范围:2~63.
//					 u32 pllp: 系统时钟的主PLL分频系数(PLL之后的分频),取值范围:2,4,6,8.(仅限这4个值!)
//					 u32 pllq: USB/SDIO/随机数产生器等的主PLL分频系数(PLL之后的分频),取值范围:2~15.
// 返 回 值: 0,成功;1,失败
// 注意事项: 外部晶振为25M的时候,推荐值:plln=360,pllm=25,pllp=2,pllq=8.
//					 Fvco:VCO频率												Fvco=Fs*(plln/pllm)													Fvco=25*(360/25)=360Mhz
//					 SYSCLK:系统时钟频率								SYSCLK=Fvco/pllp=Fs*(plln/(pllm*pllp))			SYSCLK=360/2=180Mhz
//					 Fusb:USB,SDIO,RNG等的时钟频率			Fusb=Fvco/pllq=Fs*(plln/(pllm*pllq))				Fusb=360/8=45Mhz
//-----------------------------------------------------------------
void SystemClock_Config(u32 plln,u32 pllm,u32 pllp,u32 pllq)
{
    HAL_StatusTypeDef ret = HAL_OK;
    RCC_OscInitTypeDef RCC_OscInitStructure; 
    RCC_ClkInitTypeDef RCC_ClkInitStructure;
    
    __HAL_RCC_PWR_CLK_ENABLE(); //使能PWR时钟
    
    // 下面这个设置用来设置调压器输出电压级别，以便在器件未以最大频率工作
    // 时使性能与功耗实现平衡，此功能只有STM32F42xx和STM32F43xx器件有，
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);//设置调压器输出电压级别1
    
    RCC_OscInitStructure.OscillatorType=RCC_OSCILLATORTYPE_HSE; // 时钟源为HSE
    RCC_OscInitStructure.HSEState=RCC_HSE_ON;                   // 打开HSE
    RCC_OscInitStructure.PLL.PLLState=RCC_PLL_ON;								// 打开PLL
    RCC_OscInitStructure.PLL.PLLSource=RCC_PLLSOURCE_HSE;				// PLL时钟源选择HSE
    RCC_OscInitStructure.PLL.PLLM=pllm; 												// 主PLL和音频PLL分频系数(PLL之前的分频),取值范围:2~63.
    RCC_OscInitStructure.PLL.PLLN=plln; 												// 主PLL倍频系数(PLL倍频),取值范围:64~432.  
    RCC_OscInitStructure.PLL.PLLP=pllp; 												// 系统时钟的主PLL分频系数(PLL之后的分频),取值范围:2,4,6,8.(仅限这4个值!)
    RCC_OscInitStructure.PLL.PLLQ=pllq; 												// USB/SDIO/随机数产生器等的主PLL分频系数(PLL之后的分频),取值范围:2~15.
    ret=HAL_RCC_OscConfig(&RCC_OscInitStructure);								// 初始化
	
    if(ret!=HAL_OK) while(1);
    
    ret=HAL_PWREx_EnableOverDrive(); // 开启Over-Driver功能
    if(ret!=HAL_OK) while(1);
    
    // 选中PLLCLK作为系统时钟源并且配置HCLK,PCLK1和PCLK2
    RCC_ClkInitStructure.ClockType=(RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2);
    RCC_ClkInitStructure.SYSCLKSource=RCC_SYSCLKSOURCE_PLLCLK;		 // 设置系统时钟时钟源为PLL
    RCC_ClkInitStructure.AHBCLKDivider=RCC_SYSCLK_DIV1;						 // AHB 分频系数为1，AHB 最大允许时钟频率为180MHz
    RCC_ClkInitStructure.APB1CLKDivider=RCC_HCLK_DIV4; 						 // APB1分频系数为4，APB1最大允许时钟频率为45MHz
    RCC_ClkInitStructure.APB2CLKDivider=RCC_HCLK_DIV2; 						 // APB2分频系数为2，APB2最大允许时钟频率为90MHz
    ret=HAL_RCC_ClockConfig(&RCC_ClkInitStructure,FLASH_LATENCY_5);// 同时设置FLASH延时周期为5WS，也就是6个CPU周期。
		
    if(ret!=HAL_OK) 
			while(1);
}

//-----------------------------------------------------------------
// End Of File
//----------------------------------------------------------------- 
