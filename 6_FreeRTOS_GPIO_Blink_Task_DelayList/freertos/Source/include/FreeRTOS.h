#ifndef FREERTOS_H
#define FREERTOS_H

#include "portmacro.h"
#include "list.h"
#include "FreeRTOSConfig.h"

typedef struct tskTaskControlBlock
{
	volatile StackType_t *pxTopOfStack;  //ջ����ַ
	
	ListItem_t xStateListItem;           //����ڵ�
	
	StackType_t *pxStack;                //ջ��ʼ��ַ
	
	char pctaskName[configMAX_TASK_NAME_LEN]; //�������ƣ��ַ�����ʽ
	
	TickType_t xTicksToDelay;            //������ʱ
	
	UBaseType_t uxPriority;               //���ȼ�
}tskTCB;

typedef tskTCB TCB_t; 

#define configINITIAL_TICK_COUNT    0

#endif 
