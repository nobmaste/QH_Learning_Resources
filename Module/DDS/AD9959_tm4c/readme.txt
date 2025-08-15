基于康威科技的AD9959更改，使用的单片机是tm4c12gxl
开启外设时钟之前请务必确认好自己以及配置好单片机本身的时钟

调用函数后如果无变化先去看是否调用更新函数
IO_Update();	

移植的时候更改以下内容
AD9959.h
-----------------------------------------------------------------
//AD9959管脚宏定义
// 片选与控制引脚
#define CS_HIGH()       GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, GPIO_PIN_4)
#define CS_LOW()        GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_4, 0)

#define SCLK_HIGH()     GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, GPIO_PIN_3)
#define SCLK_LOW()      GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_3, 0)

#define UPDATE_HIGH()   GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2, GPIO_PIN_2)
#define UPDATE_LOW()    GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2, 0)

// 调制配置 PS0~PS3
#define PS0_HIGH()      GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2)
#define PS0_LOW()       GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0)

#define PS1_HIGH()      GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3)
#define PS1_LOW()       GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, 0)

#define PS2_HIGH()      GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, GPIO_PIN_3)
#define PS2_LOW()       GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0)

#define PS3_HIGH()      GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, GPIO_PIN_4)
#define PS3_LOW()       GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4, 0)

// SDIO0~3
#define SDIO0_HIGH()    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, GPIO_PIN_5)
#define SDIO0_LOW()     GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_5, 0)

#define SDIO1_HIGH()    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, GPIO_PIN_6)
#define SDIO1_LOW()     GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_6, 0)

#define SDIO2_HIGH()    GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, GPIO_PIN_7)
#define SDIO2_LOW()     GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_7, 0)

#define SDIO3_HIGH()    GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_6, GPIO_PIN_6)
#define SDIO3_LOW()     GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_6, 0)

// 电源与复位控制
#define AD9959_PWR_HIGH()  GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_7, GPIO_PIN_7)
#define AD9959_PWR_LOW()   GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_7, 0)

#define RESET_HIGH()       GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_PIN_4)
#define RESET_LOW()        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_4, 0)
--------------------------------------------------------------------------------------

AD9959.c
--------------------------------------------------------------------------------------
/************************************************************
** 函数名称 ：void AD9959_Init(void)  
** 函数功能 ：初始化控制AD9959需要用到的IO口,及寄存器
** 入口参数 ：无
** 出口参数 ：无
** 函数说明 ：无
**************************************************************/
void AD9959_Init(void)  
{ 
	    // 使能 GPIOA、GPIOB、GPIOC 时钟
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    // 等待外设就绪
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA));
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB));
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOC));
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOD));
		while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF));
	
    // 配置 GPIOA 输出：PA2, PA3, PA4
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE,
        GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4);

    // 配置 GPIOB 输出：PB3
    GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE,
        GPIO_PIN_3 );

    // 配置GPIOF输出 输出：PB2,PB2,PB4

    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,
		GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4); 
		
    // 配置 GPIOC 输出：PC4,PC5,PC6,PC7
    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, 
		GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
		
		//配置GPIOD输出 输出：PD6,PD7
		GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, 
		GPIO_PIN_6|GPIO_PIN_7);
    // 初始化输出电平（可选）
		GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4, 0x00);
		GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_3, 0x00);
		GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4, 0x00);
		GPIOPinWrite(GPIO_PORTC_BASE, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7, 0x00);
		GPIOPinWrite(GPIO_PORTD_BASE, GPIO_PIN_6|GPIO_PIN_7,0X00);

		
	Intserve();  //IO口电平状态初始化
  IntReset();  //AD9959复位  
	
	//初始化功能寄存器
  AD9959_WriteData(FR1_ADD,3,FR1_DATA);//写功能寄存器1
  AD9959_WriteData(FR2_ADD,2,FR2_DATA);//
} 
---------------------------------------------------------------------------------------