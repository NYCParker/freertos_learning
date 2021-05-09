
/*
中断处理函数
*/

#include <XMC4400.h>
#include "Irq_Handle.h"
#include <xmc_gpio.h>
#include "GPIO.h"
#include "Var_Init.h"

void IRQ_Enable(void)
{
	NVIC_EnableIRQ(SysTick_IRQn);
}

void SysTick_Handler(void)
{
	LED_Count++;
	if(LED_Count==500)
	{
		XMC_GPIO_ToggleOutput(GPIO_LED);
		LED_Count = 0;
    }
}



