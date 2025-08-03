这个驱动是基于康威科技提供的AD5933.c更改的
使用的单片机是tm4c123gxl，不知道其他的ti单片机能不能用

移植的时候更改以下内容
AD5933.H
------------------------------------------------
//修改对应的端口。比方说用pb2就改成GPIO_PORTB_BASE
//修改对应的引脚不做示范 
#define AD5933_SCL_PORT GPIO_PORTE_BASE	
#define AD5933_SDA_PORT GPIO_PORTE_BASE
#define AD5933_SCL_PIN  GPIO_PIN_5		
#define AD5933_SDA_PIN  GPIO_PIN_4
------------------------------------------------
AD5933.C  同样修改对应的端口
------------------------------------------------
void Ini_I2c(void)
{
    // 使能 GPIOC 外设时钟
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    // 等待 GPIOC 模块准备好
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE));

    // 配置 PE4 (SCL) 和 PE5 (SDA) 为数字推挽输出
    GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    // 设置 PE4 和 PE5 初始状态为高电平
    GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4 | GPIO_PIN_5,GPIO_PIN_4 | GPIO_PIN_5);
}
---------------------------------------------------





