
/*************
Author:Yunchuan Ni 
0-1 FreeRTOS内核 List实现
参考FreeRTOS内核实现与应用开发实战-基于STM23
链表的实现可以从debug里看
*************/

/*
main函数
*/

#include <XMC4400.h>
#include "Irq_Handle.h"
#include "GPIO.h"
#include "Var_Init.h"
#include "List.h"

struct xLIST List_Test;       //链表根

struct xLIST_ITEM List_Item1;
struct xLIST_ITEM List_Item2;
struct xLIST_ITEM List_Item3; //三个节点

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


