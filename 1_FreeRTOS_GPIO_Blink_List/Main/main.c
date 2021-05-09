
/*************
Author:Yunchuan Ni 
0-1 FreeRTOS�ں� Listʵ��
�ο�FreeRTOS�ں�ʵ����Ӧ�ÿ���ʵս-����STM23
�����ʵ�ֿ��Դ�debug�￴
*************/

/*
main����
*/

#include <XMC4400.h>
#include "Irq_Handle.h"
#include "GPIO.h"
#include "Var_Init.h"
#include "List.h"

struct xLIST List_Test;       //�����

struct xLIST_ITEM List_Item1;
struct xLIST_ITEM List_Item2;
struct xLIST_ITEM List_Item3; //�����ڵ�

int main(void)
{
	SysTick_Config(SystemCoreClock/1000);
	GPIO_Init();
	IRQ_Enable();
	
	vListInitialise(&List_Test);
	
	vListInitialiseItem(&List_Item1);
	List_Item1.xItemValue = 1;
	
	vListInitialiseItem(&List_Item2);
	List_Item2.xItemValue = 2;
	
	vListInitialiseItem(&List_Item3);
	List_Item3.xItemValue = 3;
	
	vListInsert(&List_Test,&List_Item2);
    vListInsert(&List_Test,&List_Item3);	
    vListInsert(&List_Test,&List_Item1);	
	
	while(1)
	{
	}
}


