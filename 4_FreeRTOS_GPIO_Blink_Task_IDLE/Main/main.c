
/*************
Author:Yunchuan Ni 
0-1 FreeRTOS�ں� taskʵ��
������һ�������ĺ�����������������ѭ���Ҳ��ܷ���
�ٽ�εı���
������������
�ο�FreeRTOS�ں�ʵ����Ӧ�ÿ���ʵսָ��
*************/

/*
main����
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


