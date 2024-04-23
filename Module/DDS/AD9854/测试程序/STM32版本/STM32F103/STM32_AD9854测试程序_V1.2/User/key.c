#include "key.h"
#include "delay.h"


void Key_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE); 

  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1 |
                                GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;     
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
  GPIO_Init(GPIOE, &GPIO_InitStructure);
}


u8 Key_Scan(void)
{
  u8 rst = 0;


  if ((KEY1 == 0) || (KEY2 == 0) || (KEY3 == 0) || (KEY4 == 0))
  {
    Delay_1ms(10); 
    if (KEY1 == 0) 
    {
      rst = 1;
      while (KEY1 == 0)
        ; 
    }
    if (KEY2 == 0)
    {
      rst = 2;
      while (KEY2 == 0)
        ;
    }
    if (KEY3 == 0)
    {
      rst = 3;
      while (KEY3 == 0)
        ;
    }
    if (KEY4 == 0)
    {
      rst = 4;
      while (KEY4 == 0)
        ;
    }
  }
  else
    rst = 0;
  return rst;
}
//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
