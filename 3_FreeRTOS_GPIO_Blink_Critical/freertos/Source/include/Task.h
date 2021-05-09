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

TaskHandle_t xTaskCreateStatic(	TaskFunction_t pxTaskCode,           //������ڵ�ַ���������� ���Ͷ�����һ��ָ����������ָ��
					            const char * const pcName,           //ָ�� �������Ƶ�ַ
					            const uint32_t ulStackDepth,         //����ջ��С����λΪ��
					            void * const pvParameters,           //ָ����������� ��������������ĵ�ַ��������⣩
					            StackType_t * const puxStackBuffer,  //����ջ����ʼ��ַ
					            TCB_t * const pxTaskBuffer );        //������ƿ�ָ��
									
static void prvInitialiseNewTask( 	TaskFunction_t pxTaskCode,              //�������
									const char * const pcName,              //������
									const uint32_t ulStackDepth,            //����ջ��С
									void * const pvParameters,              //���ݸ�����Ĳ���
									TaskHandle_t * const pxCreatedTask,     //*****������
									TCB_t *pxNewTCB );                      //������ƿ�ָ��
									
void prvInitialiseTaskLists(void);
extern BaseType_t xPortStartScheduler(void);

#define taskDISABLE_INTERRUPTS()           portDISABLE_INTERRUPTS()
								
#define taskENTER_CRITICAL() portENTER_CRITICAL()
#define taskENTER_CRITICAL_FROM_ISR() portSET_INTERRUPT_MASK_FROM_ISR()

#define taskEXIT_CRITICAL() portEXIT_CRITICAL()									
#define taskEXIT_CRITICAL_FROM_ISR( x ) portCLEAR_INTERRUPT_MASK_FROM_ISR( x )	
									
#endif /* ifndef TASK_H */
