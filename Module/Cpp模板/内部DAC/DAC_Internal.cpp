#include "DAC_Internal.h"

DAC_Internal::DAC_Internal()
{
	func_init();
}

DAC_Internal::~DAC_Internal()
{
	
}

void DAC_Internal::func_init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitType;
	//使能GPIOA时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	//使能DAC时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	//Analog Mode 模拟模式
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	//下拉
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	//初始化
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	//不使用触发功能 TEN1=0
	DAC_InitType.DAC_Trigger=DAC_Trigger_None;	
	//不使用波形发生
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;
	//屏蔽、幅值设置
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0;
	//DAC1输出缓存关闭 BOFF1=1
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Disable ;
	//初始化DAC通道1	
	DAC_Init(DAC_Channel_1,&DAC_InitType);	 
	//使能DAC通道1
	DAC_Cmd(DAC_Channel_1, ENABLE);  
	//12位右对齐数据格式设置DAC值
	DAC_SetChannel1Data(DAC_Align_12b_R, 0);  
}

void DAC_Internal::DAC_Reset(int AGAIN)
{
	if(AGAIN<14)
		DAC_SetChannel1Data(DAC_Align_12b_R, (unsigned long int)((22.5*AGAIN+133)*4095/VREF));
	else if(AGAIN>=15 && AGAIN<=23)	
		DAC_SetChannel1Data(DAC_Align_12b_R, (unsigned long int)((22*AGAIN+133)*4095/VREF));
	else if(AGAIN>=24 && AGAIN<=29)
		DAC_SetChannel1Data(DAC_Align_12b_R, (unsigned long int)((21.8*AGAIN+133)*4095/VREF));
	else if(AGAIN>=30&& AGAIN<=34)
		DAC_SetChannel1Data(DAC_Align_12b_R, (unsigned long int)((21.5*AGAIN+133)*4095/VREF));
	else if(AGAIN>=35&& AGAIN<=MAX_GAIN)
		DAC_SetChannel1Data(DAC_Align_12b_R, (unsigned long int)((21.4*AGAIN+133)*4095/VREF));
}

