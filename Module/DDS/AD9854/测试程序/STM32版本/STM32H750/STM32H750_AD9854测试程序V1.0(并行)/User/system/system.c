//-----------------------------------------------------------------
// 程序描述:
//     设置系统时钟和MPU内存保护
// 作    者: 凌智电子
// 开始日期: 2020-11-11
// 完成日期: 2020-11-11
// 修改日期: 
// 当前版本: V1.0
// 历史版本:
//  - V1.0: (2020-11-11)设置系统时钟和MPU内存保护
// 调试工具: 凌智STM32H750核心板、LZE_ST_LINK2
// 说    明: 
//    
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// 头文件包含
//-----------------------------------------------------------------
#include "system.h"
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// void SystemClock_Config(void)
//-----------------------------------------------------------------
// 
// 函数功能: 系统时钟初始化
// 入口参数: 无
// 返 回 值: 无
// 注意事项:
//            System Clock source  = PLL (HSE) 25MHz晶振
//	          SYSCLK(Hz)           = 400000000 (CPU Clock)
//	          HCLK(Hz)             = 200000000 (AXI and AHBs Clock)
//	          AHB Prescaler        = 2
//	          D1 APB3 Prescaler    = 2 (APB3 Clock  100MHz)
//	          D2 APB1 Prescaler    = 2 (APB1 Clock  100MHz)
//	          D2 APB2 Prescaler    = 2 (APB2 Clock  100MHz)
//	          D3 APB4 Prescaler    = 2 (APB4 Clock  100MHz)
//	          HSE Frequency(Hz)    = 25000000
//	          PLL_M                = 5
//	          PLL_N                = 160
//	          PLL_P                = 2
//	          PLL_Q                = 4
//	          PLL_R                = 2
//	          VDD(V)               = 3.3
//	          Flash Latency(WS)    = 2
//
//-----------------------------------------------------------------
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  MODIFY_REG(PWR->CR3, PWR_CR3_SCUEN, 0); // 供应配置更新启用
  // 下面这个设置用来设置调压器输出电压级别，让器件以最大频率工作
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);	//设置调压器输出电压级别1

  while ((PWR->D3CR & (PWR_D3CR_VOSRDY)) != PWR_D3CR_VOSRDY) 
  {
    
  }
  // 初始化CPU，AHB和APB总线时钟
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;	// 时钟源为HSE
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;										// 打开HSE
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;								// 打开PLL
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;				// PLL时钟源选择HSE
  RCC_OscInitStruct.PLL.PLLM = 5;															// 主PLL和音频PLL分频系数(PLL之前的分频),取值范围:1~63.
  RCC_OscInitStruct.PLL.PLLN = 160;														// 主PLL倍频系数(PLL倍频),取值范围:4~512.  
  RCC_OscInitStruct.PLL.PLLP = 2;															// 系统时钟的主PLL分频系数(PLL之后的分频)
  RCC_OscInitStruct.PLL.PLLQ = 4;															// 外设时钟的分频系数Q
  RCC_OscInitStruct.PLL.PLLR = 2;															// 外设时钟的分频系数R
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;					// PLL时钟输入范围
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;					// PLL时钟输出范围
  RCC_OscInitStruct.PLL.PLLFRACN = 0;													// 指定PLL1 VCO的乘数的小数部分。它应该是0到8191之间的值
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)				// 初始化
  {
    while(1) 
		{ 
			; 
		}
  }
  
	// 选中PLLCLK作为系统时钟源并且配置HCLK,PCLK1和PCLK2
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;	// 作为系统时钟的时钟源
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;				// 系统时钟分频器
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;					// AHB时钟（HCLK）分频器，该时钟来自系统时钟（SYSCLK）
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;					// APB3时钟（D1PCLK1）分频器，该时钟源自AHB时钟（HCLK）
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;					// APB1时钟（PCLK1）分频器，该时钟源自AHB时钟（HCLK）
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;					// APB2时钟（PCLK2）分频器，该时钟源自AHB时钟（HCLK）
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;					// APB4时钟（D3PCLK1）分频器，该时钟源自AHB时钟（HCLK）

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)	
  {
    while(1) 
		{ 
			; 
		}
  }
}

//-----------------------------------------------------------------
// void CPU_CACHE_Enable(void)
//-----------------------------------------------------------------
// 
// 函数功能: 启用CPU缓存
// 入口参数: 无
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void CPU_CACHE_Enable(void)
{
  SCB_EnableICache();	// 使能指令缓存
  SCB_EnableDCache();	// 使能数据缓存
	SCB->CACR|=1<<2;		// 设置D-Cache为Write Through，保证数据的一致性
}

//-----------------------------------------------------------------
// void MPU_Set_Protection(u32 Number,u32 BASEADDR,u32 SIZE,u8 AP,u8 DE,u8 S,u8 C,u8 B)
//-----------------------------------------------------------------
// 
// 函数功能: 配置MPU
// 入口参数: u32 Number：配置保护区域
//					 u32 BASEADDR：设置基地址
//					 u32 SIZE： 设置保护区域的大小
//					 u8 AP：设置访问权限
//					 u8 DE：是否允许指令访问
//					 u8 S：是否共享
//					 u8 C：是否缓存
//					 u8 B：是否缓冲
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void MPU_Set_Protection(u32 Number,u32 BASEADDR,u32 SIZE,u8 AP,u8 DE,u8 S,u8 C,u8 B)
{
	MPU_Region_InitTypeDef MPU_Initure;
	
	HAL_MPU_Disable(); 											// 配置 MPU 之前先关闭 MPU,配置完成以后在使能 MPU
	MPU_Initure.Enable=MPU_REGION_ENABLE; 	// 使能该保护区域
	MPU_Initure.Number=Number; 							// 设置保护区域
	MPU_Initure.BaseAddress=BASEADDR; 			// 设置基址
	MPU_Initure.Size=SIZE; 									// 设置保护区域大小
	MPU_Initure.SubRegionDisable=0X00; 			// 禁止子区域
	MPU_Initure.TypeExtField=MPU_TEX_LEVEL0;// 设置类型扩展域为 level0
	MPU_Initure.AccessPermission=AP; 				// 设置访问权限,
	MPU_Initure.DisableExec=DE; 						// 是否允许指令访问(允许读取指令)
	MPU_Initure.IsShareable=S; 							// 是否共享
	MPU_Initure.IsCacheable=C; 							// 是否缓存
	MPU_Initure.IsBufferable=B; 						// 是否缓冲
	HAL_MPU_ConfigRegion(&MPU_Initure); 		// 配置 MPU
	HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT); // 开启 MPU
}

//-----------------------------------------------------------------
// void MPU_Memory_Protection(void)
//-----------------------------------------------------------------
// 
// 函数功能: 设置需要的保护区域，AXI SRAM和FMC
// 入口参数: 无
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void MPU_Memory_Protection(void)
{
	// 配置AXI SRAM，共512KB字节，全访问，允许指令访问，禁止共享，允许缓存，允许缓冲
	MPU_Set_Protection(
		MPU_REGION_NUMBER0,
		0x24000000,						
		MPU_REGION_SIZE_512KB,
		MPU_REGION_FULL_ACCESS,
		MPU_INSTRUCTION_ACCESS_ENABLE,
		MPU_ACCESS_NOT_SHAREABLE,
		MPU_ACCESS_CACHEABLE,
		MPU_ACCESS_BUFFERABLE
	);
	
	// 配置FMC，共64MB字节，全访问，允许指令访问，禁止共享，禁止缓存，允许缓冲
	MPU_Set_Protection(
		MPU_REGION_NUMBER1,
		0x60000000,				
		MPU_REGION_SIZE_64MB,
		MPU_REGION_FULL_ACCESS,
		MPU_INSTRUCTION_ACCESS_ENABLE,
		MPU_ACCESS_NOT_SHAREABLE,
		MPU_ACCESS_NOT_CACHEABLE,
		MPU_ACCESS_BUFFERABLE
	);
}

//-----------------------------------------------------------------
// End Of File
//----------------------------------------------------------------- 
