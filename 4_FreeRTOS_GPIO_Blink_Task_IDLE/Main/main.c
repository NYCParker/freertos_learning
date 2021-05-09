
/*************
Author:Yunchuan Ni 
0-1 FreeRTOS内核 task实现
任务是一个独立的函数，函数主体无限循环且不能返回
临界段的保护
创建空闲任务
参考FreeRTOS内核实现与应用开发实战指南
*************/

/*
main函数
*/

#include <XMC4400.h>
#include "Irq_Handle.h"
#include "GPIO.h"
#include <xmc_gpio.h>
#include "Var_Init.h"
#include "List.h"
#include "Sys_Task.h"

int main(void)
{
	SysTick_Config(SystemCoreClock/1000);
	GPIO_Init();
	
	LED_Blink_Task();
	
	while(1)
	{
		
	}
}


