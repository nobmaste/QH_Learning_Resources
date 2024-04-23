/**
 **********************************************************************
 *@Copyright(c)		Lance Zhang
 *@Identification	CHIC
 *@Describtion		Template
 *
 *@PresentVersion	1.2
 *@Writer			Lance Zhang
 *@Date				7/10/2019
 **********************************************************************/
/**********************************************************************
 *@Instruction
 *基本使用方法：
 *
 *1、调用Gen_Input()函数，将采集到的电压值(int)
 *	 转换为供FFT计算使用的输入数组
 *
 *2、调用FFT()函数，对输入数组进行复数傅里叶变换并输出
 *
 *3、调用output_scan()函数，可以对傅里叶变换以后的数组进行扫描
 *	 并获得输出数组的基本特性
 **********************************************************************/
/**********************************************************************
 *@Attention		输入数组的大小应是输出数组大小的2倍
 *@Attention		由于采用基4运算，输入和输出数组应为4的整数倍
 *@Attention		所谓的形态学滤波不同环境下需要具体配置，尽量自己写
 **********************************************************************
 *@Compatibility	调用了arm_math.h，没别的了
 *
 *整体兼容性：高
 *
 *@Characteristic	output_scan()函数主要求得输出数组的
 *					平均值，最大值，最小值，频率					
 *
 *@Warning	#include "stm32f4xx.h"必须放在arm_math.h之前进行初始化
 *			否则会出现如下警告：
 **********************************************************************
 *		#warning "Compiler generates FPU instructions for a device 
 *					without an FPU (check __FPU_PRESENT)"
 **********************************************************************/
        
#ifndef	_CFFT_H
#define _CFFT_H
extern "C"
{
	#include "stm32f4xx.h"
	#include "arm_math.h"
	#include "sys.h"
}

class	CFFT
{
public:
	CFFT();
	~CFFT();
public:
	//傅里叶逆变换
	void IFFT(float* fft_inputbuf,float* fft_outputbuf,u16 input_size = 1024);
	//傅里叶变换
	void FFT(float* fft_inputbuf,float* fft_outputbuf,u16 input_size = 1024);
public:
	void Gen_Input(u16* vol_input, float* fft_input,u16 size = 1024);
	//扫描傅里叶变换后的输出数组得到平均值，最大值，最小值，频率等信息
	void output_scan(float* fft_output, float Fs = 50000, u16 size = 1024);
//原信号状态，目前就F稍微准一点
public:
	float max_val;
	float min_val;
	float freq_val;
	float AM_val;
	float DC_val;
public:
	void debug_output_scan(float* fft_output, float Fs = 50000, u16 size = 1024);
	u16 debug_max_loc;
	u16 debug_max_val;
	u16 debug_str_val;
	float debug_mid_val;
	u16 debug_ave_val;
	u16 debug_freq;
//形态学滤波
public:
	//均值滤波,默认偏移为0
	void blur(float* fft_output,u16 size = 1024, float bias = 0);
//形态学因子
private:
	//均值滤波因子
	float factor_ave;
};

/**
 **********************************************************************
 *@Copyright(c)		Lance Zhang
 *@Identification	CHIC
 *@Describtion		Template
 *
 *@PresentVersion	1.2
 *@Writer			Lance Zhang
 *@Date				7/10/2019
 **********************************************************************/
/************************************************* *********************
 *@Instruction
 *Basic use:
 *
 *1, call the Gen_Input() function, and the collected voltage value (int)
 * Converted to an input array for FFT calculations
 *
 *2, call the FFT () function, perform complex Fourier transform on the input array and output
 *
 *3, call the output_scan () function, you can scan the array after the Fourier transform
 * and get the basic characteristics of the output array
 ************************************************** ********************/
/************************************************* *********************
 *@Attention The size of the input array should be twice the size of the output array
 *@Attention The input and output arrays should be integer multiples of 4 due to base 4 operations.
 *@Attention The so-called morphological filtering requires specific 
 *			 configuration in different environments, try to write by yourself
 ************************************************** ********************
 *@Compatibility called arm_math.h, nothing else
 *
 *@Overall compatibility: high
 *
 *@Characteristic output_scan() function main request output array
 * Average, maximum, minimum, frequency
 *
 *@Warning #include "stm32f4xx.h" must be initialized before arm_math.h
 * Otherwise the following warning will appear:
 **********************************************************************
 *		#warning "Compiler generates FPU instructions for a device 
 *					without an FPU (check __FPU_PRESENT)"
 **********************************************************************/
 
 /**
  **********************************************************************
  * @Copyright(c) Ланс Чжан
  * @Идентификация CHIC
  * @Describtion шаблона
  *
  * @Настоящая версия 1.2
  * @Писатель Ланс Чжан
  * @Дата 7/10/2019
  **********************************************************************/
/**********************************************************************
 * @Инструкция
 * Основное использование:
 *
 * 1, вызовите функцию Gen_Input () и полученное значение напряжения (int)
 * Преобразован во входной массив для расчетов БПФ
 *
 * 2, вызвать функцию FFT (), выполнить комплексное преобразование Фурье для входного массива и вывести
 *
 * 3, вызовите функцию output_scan (), вы можете сканировать массив после преобразования Фурье
 * и получить основные характеристики выходного массива
 **********************************************************************/
/************************************************* *********************
 * @Внимание Размер входного массива должен быть в два раза больше выходного массива
 * @Внимание Массивы ввода и вывода должны быть целыми числами, кратными 4, из-за операций с базой 4.
 * @Внимание Так называемая морфологическая фильтрация требует определенной конфигурации в разных средах, попробуйте написать самостоятельно
 ************************************************** ********************
 * @Совместимость называется arm_math.h, больше ничего
 *
 * Общая совместимость: высокая
 *
 * @Характеристика output_scan () функция выходного массива основного запроса
 * Средняя, максимальная, минимальная, частота
 *
 * @Предупреждение #include "stm32f4xx.h" должно быть инициализировано до arm_math.h
 * В противном случае появится следующее предупреждение:
 *
 **********************************************************************
 *		#warning "Compiler generates FPU instructions for a device 
 *					without an FPU (check __FPU_PRESENT)"
 **********************************************************************/
#endif
