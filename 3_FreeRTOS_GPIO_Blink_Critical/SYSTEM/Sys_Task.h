#ifndef SYSTASK_H
#define SYSTASK_H

#include "list.h"
#include "Task.h"
#include "port.h"

#define taskYIELD()  portYIELD()

extern List_t pxReadyTasksList[configMAX_PRIORITIES];  //ͬһ���ȼ�������Ͳ��뵽�����б��ͬһ��������
extern TCB_t Task1TCB;
extern TCB_t Task2TCB;
extern void vTaskStartScheduler(void);
	
void Task1_Entry(void *p_arg);
void Task2_Entry(void *p_arg);
void LED_Blink_Task(void);
#endif 

