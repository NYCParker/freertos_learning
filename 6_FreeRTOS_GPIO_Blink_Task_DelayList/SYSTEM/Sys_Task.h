#ifndef SYSTASK_H
#define SYSTASK_H

#include "list.h"
#include "Task.h"
#include "port.h"

#define taskYIELD()  portYIELD()

extern void vApplicationGetIdleTaskMemory( TCB_t **ppxIdleTaskTCBBuffer,   //传入一个指针的地址
									StackType_t **ppxIdleTaskStackBuffer,  //传入一个指针的地址
									uint32_t *pulIdleTaskStackSize );      //传入一个地址

extern List_t pxReadyTasksLists[configMAX_PRIORITIES];  //同一优先级的任务就插入到就绪列表的同一条链表中
extern TCB_t Task1TCB;
extern TCB_t Task2TCB;
extern TCB_t IdleTaskTCB;  
extern void vTaskStartScheduler(void);

void Task1_Entry(void *p_arg);
void Task2_Entry(void *p_arg);
void LED_Blink_Task(void);
#endif 

