
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
	NVIC_SetPriority(SysTick_IRQn,0);
}

void SysTick_Handler(void)
{
}



