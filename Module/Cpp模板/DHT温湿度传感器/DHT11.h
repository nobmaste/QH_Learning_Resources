#ifndef __DHT_11
#define	__DHT_11

extern "C"{
	#include "stm32f4xx_gpio.h"
	#include "delay.h"
}
#include "LCD_CPP.h"

#define DHT11_OUT_H		(PEout(0) = 1)
#define DHT11_OUT_L		(PEout(0) = 0)
#define DHT11_IN		 PEin(0)

//用的端口是PE0
class DHT11
{
private:
	void DHT11_GPIO_Output();
	void DHT11_GPIO_Input();
	void DHT11_reset();

	u16 DHT11_Scan();
	u16 DHT11_Read_Bit();
	u16 DHT11_Read_Byte();

public:
	DHT11();
	~DHT11();
	void Func_Activate();

	u16 DHT11_Read_Data(u8 buffer[5]);

	char temp_value[3];
	double temp;
	double hum;
};

#endif /*__DHT_11*/
