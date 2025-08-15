#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_gpio.h"
#include "driverlib/gpio.h"
#include "driverlib/systick.h"
#include "driverlib/pin_map.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"



void HardFault_Handler(void)
{
    while(1);
}

