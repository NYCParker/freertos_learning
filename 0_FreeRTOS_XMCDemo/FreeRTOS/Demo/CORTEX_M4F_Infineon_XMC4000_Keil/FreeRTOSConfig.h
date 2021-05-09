/*
 * FreeRTOS V202012.00
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */


#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H


/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *
 * See http://www.freertos.org/a00110.html
 *----------------------------------------------------------*/

#include <stdint.h>
extern uint32_t SystemCoreClock;

/* The following definition allows the startup files that ship with the IDE
to be used without modification when the chip used includes the PMU CM001
errata. */
/*Config��ʼ�ĺꡣ��Щ���������freertos.h
TIME SLICING ʹ��ʱ��Ƭ���ȣ�1�����ȼ����Զ���������
TICKLESS_IDLE_ �͹���TICKLESSģʽ ����û�ҵ�
*/

#define WORKAROUND_PMU_CM001					1

#define configUSE_PORT_OPTIMISED_TASK_SELECTION	1
#define configUSE_PREEMPTION					1          //1ʹ����ռʽ�ں�  0ʹ��Э�� ������1 �����ȼ����������ռ�����ȼ�����
#define configUSE_PORT_OPTIMISED_TASK_SELECTION	1          //�������ⷽ��ѡ����һ��Ҫ���е����� һ����Ӳ������ǰ����ָ��

/***************************************************************************************************************/
/*                                FreeRTOS���ڴ������й�����ѡ��                                                */
/***************************************************************************************************************/
#define configTOTAL_HEAP_SIZE					( ( size_t ) ( 22800 ) )   //ϵͳ�����ܵĶѴ�С ���˿���û��������ʾ�ڴ治��
//��Ȼʵ�����ڴ������Ҫ��һ���ڴ�أ���heap.c
//PRIVILEGED_DATA static uint8_t ucHeap[ configTOTAL_HEAP_SIZE ]; heap4

/***************************************************************************************************************/
/*                                FreeRTOS�빳�Ӻ����йص�����ѡ��                                              */
/***************************************************************************************************************/
#define configUSE_IDLE_HOOK						0  //1��ʹ�ÿ��й��ӣ�0����ʹ��
#define configUSE_TICK_HOOK						0  //1��ʹ��ʱ��Ƭ���ӣ�0����ʹ��

/***************************************************************************************************************/
/*                                FreeRTOS������ʱ�������״̬�ռ��йص�����ѡ��                                 */
/***************************************************************************************************************/
#define configUSE_TRACE_FACILITY				1  //Ϊ1���ÿ��ӻ����ٵ���



#define configCPU_CLOCK_HZ						( SystemCoreClock )        //CPUƵ��
#define configTICK_RATE_HZ						( ( TickType_t ) 1000 )    //ʱ�ӽ���Ƶ�ʣ���������Ϊ1000�����ھ���1ms
#define configMAX_PRIORITIES					( 5 )                      //��ʹ�õ�������ȼ�
#define configMINIMAL_STACK_SIZE				( ( unsigned short ) 130 ) //��������ʹ�õĶ�ջ��С
	
#define configMAX_TASK_NAME_LEN					( 10 )                     //���������ַ�������
#define configUSE_16_BIT_TICKS					0                          //ϵͳ���ļ����������������ͣ�1��ʾΪ16λ�޷������Σ�0��ʾΪ32λ�޷�������
#define configIDLE_SHOULD_YIELD					1                          //Ϊ1ʱ�����������CPUʹ��Ȩ������ͬ���ȼ����û�����
                                                                           //freertos���ʲô����û�У��ͻ�ִ�п������񣬶���ûʲôҪ��Ӧ�������
#define configUSE_MUTEXES						1                          //Ϊ1ʱʹ�û����ź���
#define configQUEUE_REGISTRY_SIZE				8                          //��Ϊ0ʱ��ʾ���ö��м�¼�������ֵ�ǿ��Լ�¼�Ķ��к��ź��������Ŀ��
#define configCHECK_FOR_STACK_OVERFLOW			2                          //����0ʱ���ö�ջ�����⹦�ܣ����ʹ�ô˹����û������ṩһ��ջ������Ӻ�����
                                                                           //����Ϊ1/2 ������ջ�����ⷽ��������ƷҪ�ص���
#define configUSE_RECURSIVE_MUTEXES				1                          //Ϊ1ʱʹ�õݹ黥���ź���
#define configUSE_MALLOC_FAILED_HOOK			1                          //1 ʹ���ڴ�����ʧ�ܹ��Ӻ���
#define configUSE_APPLICATION_TASK_TAG			0                          //�û������ǩ
#define configUSE_COUNTING_SEMAPHORES			1                          //Ϊ1ʱʹ�ü����ź���
#define configGENERATE_RUN_TIME_STATS			0

/* Co-routine definitions. */
/***************************************************************************************************************/
/*                                FreeRTOS��Э���йص�����ѡ��                                                  */
/***************************************************************************************************************/
#define configUSE_CO_ROUTINES 		0                 //Ϊ1ʱ����Э�̣�����Э���Ժ��������ļ�croutine.c
#define configMAX_CO_ROUTINE_PRIORITIES ( 2 )         //Э�̵���Ч���ȼ���Ŀ

/* Software timer definitions. */
/***************************************************************************************************************/
/*                                FreeRTOS�������ʱ���йص�����ѡ��                                            */
/***************************************************************************************************************/
#define configUSE_TIMERS				1             //Ϊ1ʱ���������ʱ��
#define configTIMER_TASK_PRIORITY		( 2 )         //�����ʱ�����ȼ� //(configMAX_PRIORITIES-1)
#define configTIMER_QUEUE_LENGTH		5             //�����ʱ�����г���
#define configTIMER_TASK_STACK_DEPTH	( configMINIMAL_STACK_SIZE * 2 )  //�����ʱ�������ջ��С

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */

/*INCLUDE��ʼ�ĺ�
���統��INCLUDE_vTaskPrioritySet����Ϊ0��ʱ�򣬱�ʾ����ʹ�ú���vTaskPrioritySet()
#if ( INCLUDE_vTaskPrioritySet == 1 )  ��task.c��

    void vTaskPrioritySet( TaskHandle_t xTask,
                           UBaseType_t uxNewPriority )
{.....................
FreeRTOS�еĲü������þ�����������������ķ�����ʵ�ֵģ���ʡ�ռ䣬����ʹ�õ�MCU������ϵͳ����
���Ҫʹ�ú�������Ҫ����Ӧ�ĺ궨��Ϊ1
 */
 
#define INCLUDE_vTaskPrioritySet		1
#define INCLUDE_uxTaskPriorityGet		1
#define INCLUDE_vTaskDelete				1
#define INCLUDE_vTaskCleanUpResources	1
#define INCLUDE_vTaskSuspend			1
#define INCLUDE_vTaskDelayUntil			1
#define INCLUDE_vTaskDelay				1

/* Cortex-M specific definitions. */
/***************************************************************************************************************/
/*                                FreeRTOS���ж��йص�����ѡ��                                                  */
/***************************************************************************************************************/

#ifdef __NVIC_PRIO_BITS
	/* __BVIC_PRIO_BITS will be specified when CMSIS is being used. */
	#define configPRIO_BITS       		__NVIC_PRIO_BITS
#else
	#define configPRIO_BITS       		6        /* 63 priority levels */
#endif

/* The lowest interrupt priority that can be used in a call to a "set priority"
function. */
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY			0x3f           //63 �ж�������ȼ�

/* The highest interrupt priority that can be used by any interrupt service
routine that makes calls to interrupt safe FreeRTOS API functions.  DO NOT CALL
INTERRUPT SAFE FREERTOS API FUNCTIONS FROM ANY INTERRUPT THAT HAS A HIGHER
PRIORITY THAN THIS! (higher priorities are lower numeric values. */
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY	5              //ϵͳ�ɹ��������ж����ȼ�

/* Interrupt priorities used by the kernel port layer itself.  These are generic
to all Cortex-M ports, and do not rely on any particular library functions. */
#define configKERNEL_INTERRUPT_PRIORITY 		( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) ) //XMCʹ��6λ��Ϊ���ȼ����ڸ�6λ����Ҫ����2λȥ����
//��FreeRTOS��PendSV��SysTick���ж����ȼ�������͵�
/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) ) //���ȼ�����С��5����RTOS����
//�˺����ú��Ժ󣬸��ڴ����ȼ����ж�RTOS�ǲ��ܽ�ֹ�ģ��ж˷�����Ҳ���ܵ���FreeRTOS��API����
	
/* Normal assert() semantics without relying on the provision of an assert.h
header file. */
#define configASSERT( x ) if( ( x ) == 0 ) { taskDISABLE_INTERRUPTS(); for( ;; ); }	
	
/* Definitions that map the FreeRTOS port interrupt handlers to their CMSIS
standard names.  WORKAROUND_PMU_CM001 is defined at the top of this file. */
/***************************************************************************************************************/
/*                                FreeRTOS���жϷ������йص�����ѡ��                                          */
/***************************************************************************************************************/
#if WORKAROUND_PMU_CM001 == 1
	#define xPortPendSVHandler PendSV_Handler_Veneer
#else
	#define xPortPendSVHandler PendSV_Handler
#endif
#define vPortSVCHandler SVC_Handler
#define xPortSysTickHandler SysTick_Handler


/* Demo application specific settings. */
#if defined( PART_XMC4500 )
	/* Hardware includes. */
	#include "XMC4500.h"
	#include "System_XMC4500.h"

	/* Configure pin P3.9 for the LED. */
	#define configCONFIGURE_LED() ( PORT3->IOCR8 = 0x00008000 )
	/* To toggle the single LED */
	#define configTOGGLE_LED()	( PORT3->OMR =	0x02000200 )
#elif defined( PART_XMC4400 )
	/* Hardware includes. */
	
	#include "XMC4400.h"
	#include "System_XMC4200.h"

	/* Configure pin P5.2 for the LED. */
	#define configCONFIGURE_LED() ( PORT5->IOCR0 = 0x00800000 )
	/* To toggle the single LED */
	#define configTOGGLE_LED()	( PORT5->OMR =	0x00040004 )
	
#elif defined( PART_XMC4200 )
	/* Hardware includes. */
	#include "XMC4200.h"
	#include "System_XMC4200.h"

	/* Configure pin P2.1 for the LED. */
	#define configCONFIGURE_LED() PORT2->IOCR0 = 0x00008000; PORT2->HWSEL &= ~0x0000000cUL
	/* To toggle the single LED */
	#define configTOGGLE_LED()	( PORT2->OMR =	0x00020002 )
#else
	#error Part number not specified in project options
#endif


#endif /* FREERTOS_CONFIG_H */

