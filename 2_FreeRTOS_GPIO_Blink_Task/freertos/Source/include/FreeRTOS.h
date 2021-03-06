#ifndef FREERTOS_H
#define FREERTOS_H

#include "portmacro.h"
#include "list.h"
#include "FreeRTOSConfig.h"

typedef struct tskTaskControlBlock
{
	volatile StackType_t *pxTopOfStack;  //栈顶地址
	
	ListItem_t xStateListItem;           //任务节点
	
	StackType_t *pxStack;                //栈起始地址
	
	char pctaskName[configMAX_TASK_NAME_LEN]; //任务名称，字符串形式
}tskTCB;

typedef tskTCB TCB_t; 

#endif 
