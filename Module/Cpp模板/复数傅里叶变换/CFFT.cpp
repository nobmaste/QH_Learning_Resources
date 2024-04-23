#include "CFFT.h"

CFFT::CFFT()
{
	factor_ave = 0;
	max_val = 0;
	min_val = 0;
	freq_val = 0;	
	AM_val = 0;
	DC_val = 0;
	
	debug_max_loc = 0;
	debug_max_val = 0;
	debug_str_val = 0;
	debug_mid_val = 0;
	debug_ave_val = 0;

	debug_freq = 0;
}

CFFT::~CFFT()
{
	
}
//生成fft_inputbuf[]数组，将采样到的电压值输入傅里叶输入数组
void CFFT::Gen_Input(u16* vol_input, float* fft_input,u16 size)
{
	for(int i=0;i<size;i++)									//生成信号序列
	{
		fft_input[2*i] = (vol_input[i]/4096.0)*3.3;			//生成输入信号实部
		fft_input[2*i+1]=0;									//虚部全部为0
	}
}
//傅里叶变换
void CFFT::FFT(float* fft_inputbuf,float* fft_outputbuf,u16 input_size)
{
	arm_cfft_radix4_instance_f32 	scfft;
	arm_cfft_radix4_init_f32(&scfft,input_size,0,1);
	arm_cfft_radix4_f32(&scfft,fft_inputbuf);					//FFT计算（基4）
	arm_cmplx_mag_f32(fft_inputbuf,fft_outputbuf,input_size);	//把运算结果复数求模得幅值 		
}
//傅里叶逆变换
void CFFT::IFFT(float* fft_inputbuf,float* fft_outputbuf,u16 input_size)
{
	arm_cfft_radix4_instance_f32 	scfft;
	arm_cfft_radix4_init_f32(&scfft,input_size,1,1);
	arm_cfft_radix4_f32(&scfft,fft_inputbuf);					//FFT计算（基4）
	arm_cmplx_mag_f32(fft_inputbuf,fft_outputbuf,input_size);	//把运算结果复数求模得幅值 
}
//输入FFT输出的数组，输入采样频率
void CFFT::output_scan(float* fft_output, float Fs, u16 size)
{
	//获取频率系数freq_temp = (n-1)/N
	float freq_temp = 0;	
	float am_temp = 0;
	//先获取直流分量
	float dc_value = fft_output[0];
	//得到最大值所在的位置
	u32 max_location;
	
	//获得直流分量
	DC_val = dc_value/size;
	
	//将直流分量去除,后面记得加回去
	fft_output[0] = 0;
	//求前一半数组中的最大值
	arm_max_f32(fft_output,512,&am_temp,&max_location);
	//将直流分量还原
	fft_output[0] = dc_value;
	//求频率,幅值
	freq_temp = (float)max_location/size;
	//进行判断，如果交流信号的幅值小于了max_val/(size/2),则视为直流分量
	//size = 1024的情况下为90mV
	if(fft_output[max_location] <= 500)
	{
		freq_val = 0;
		debug_freq = 0;
	}
	else
	{
		freq_val = freq_temp*Fs;
		debug_freq = 1;
	}
	AM_val = 2*(am_temp/size);
	//最大最小值
	max_val = DC_val + AM_val;
	min_val = DC_val - AM_val;
}

void CFFT::debug_output_scan(float* fft_output, float Fs, u16 size)
{
	//获取频率系数freq_temp = (n-1)/N
	float freq_temp = 0;	
	float am_temp = 0;
	//先获取直流分量
	float dc_value = fft_output[0];
	debug_str_val = dc_value;
	//得到最大值所在的位置
	u32 max_location;
	
	//获得直流分量
	//DC_val = dc_value/size;
	
	//将直流分量去除,后面记得加回去
	fft_output[0] = 0;
	//求前一半数组中的最大值
	arm_max_f32(fft_output,512,&am_temp,&max_location);
	//将直流分量还原
	fft_output[0] = dc_value;
	//得到最大值的坐标
	debug_max_val = fft_output[max_location];
	debug_max_loc = max_location;
	
	//求频率,幅值，最大最小值
	freq_temp = (float)max_location/size;
	freq_val = freq_temp*Fs;
	AM_val = 2*(am_temp/size);
	max_val = DC_val + AM_val;
	min_val = DC_val - AM_val;
}

//均值滤波
void CFFT::blur(float* fft_output, u16 size, float bias)
{
	//求平均值,获得均值因子
	arm_mean_f32(fft_output,512,&factor_ave);
	
	for(int i=0;i<size;i++)
	{
		if(factor_ave + bias > fft_output[i])
		{
			fft_output[i] = 0;
		}			
	}
}
