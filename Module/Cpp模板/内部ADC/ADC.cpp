#include "ADC.h"

ADC_Internal::ADC_Internal(bool interrupt_used)
{
	if(interrupt_used)
	{
		TIM3_Int_Init();
		extern u16	VOLTAGE;
		vol = &VOLTAGE;		
	}
	func_Init();
	Get_Adc();
}

ADC_Internal::~ADC_Internal()
{
	
}

void ADC_Internal::func_Init()
{
	GPIO_InitTypeDef  		GPIO_InitStructure;
	ADC_CommonInitTypeDef 	ADC_CommonInitStructure;
	ADC_InitTypeDef       	ADC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 

	//先初始化ADC1通道5 IO口
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	//不带上下拉
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init(GPIOA, &GPIO_InitStructure);  

	//ADC1复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  
	//复位结束	 
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	

	//独立模式
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
	//两个采样阶段之间的延迟5个时钟
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
	//DMA失能
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled; 
	//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;
	//初始化
	ADC_CommonInit(&ADC_CommonInitStructure);
	//12位模式
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	//非扫描模式	
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	//关闭连续转换
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	//禁止触发检测，使用软件触发
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	//右对齐	
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	//1个转换在规则序列中 也就是只转换规则序列1 
	ADC_InitStructure.ADC_NbrOfConversion = 1;
	ADC_Init(ADC1, &ADC_InitStructure);
	//开启AD转换器
	ADC_Cmd(ADC1, ENABLE);	
}

u16 ADC_Internal::Get_Adc(u8 ch)   
{
	//设置指定ADC的规则组通道，一个序列，采样时间
	//ADC1,ADC通道,480个周期,提高采样时间可以提高精确度	
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles );			    
	//使能指定的ADC1的软件转换启动功能	
	ADC_SoftwareStartConv(ADC1);		
	//等待转换结束
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));
	//返回最近一次ADC1规则组的转换结果
	return ADC_GetConversionValue(ADC1);	
}

u16 ADC_Internal::Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		//delay_ms(1);
	}
	return temp_val/times;
} 
