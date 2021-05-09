#ifndef TASK_H
#define TASK_H

#include "portmacro.h"
#include "projdefs.h"
#include "FreeRTOS.h"
#include "portmacro.h"
#include "list.h"
#include "port.h"
#include "Sys_Task.h"

typedef void * TaskHandle_t;

TaskHandle_t xTaskCreateStatic(	TaskFunction_t pxTaskCode,           //任务入口地址（函数名） 类型定义了一个指向任务函数的指针
					            const char * const pcName,           //指针 任务名称地址
					            const uint32_t ulStackDepth,         //任务栈大小，单位为字
					            void * const pvParameters,           //指向传入任务参数 （传入任务参数的地址，方便理解）
					            StackType_t * const puxStackBuffer,  //任务栈的起始地址
					            TCB_t * const pxTaskBuffer );        //任务控制块指针
									
static void prvInitialiseNewTask( 	TaskFunction_t pxTaskCode,              //任务入口
									const char * const pcName,              //函数名
									const uint32_t ulStackDepth,            //任务栈大小
									void * const pvParameters,              //传递给任务的参数
									TaskHandle_t * const pxCreatedTask,     //*****任务句柄
									TCB_t *pxNewTCB );                      //任务控制块指针
									
void prvInitialiseTaskLists(void);
extern BaseType_t xPortStartScheduler(void);

#define taskDISABLE_INTERRUPTS()           portDISABLE_INTERRUPTS()
								
#define taskENTER_CRITICAL() portENTER_CRITICAL()
#define taskENTER_CRITICAL_FROM_ISR() portSET_INTERRUPT_MASK_FROM_ISR()

#define taskEXIT_CRITICAL() portEXIT_CRITICAL()									
#define taskEXIT_CRITICAL_FROM_ISR( x ) portCLEAR_INTERRUPT_MASK_FROM_ISR( x )	
									
#endif /* ifndef TASK_H */
