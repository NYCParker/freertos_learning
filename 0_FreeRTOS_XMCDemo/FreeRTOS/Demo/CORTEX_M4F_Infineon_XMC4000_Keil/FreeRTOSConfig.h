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
/*Config开始的宏。有些不在这里，在freertos.h
TIME SLICING 使能时间片调度，1个优先级可以定义多个任务
TICKLESS_IDLE_ 低功耗TICKLESS模式 好像没找到
*/

#define WORKAROUND_PMU_CM001					1

#define configUSE_PORT_OPTIMISED_TASK_SELECTION	1
#define configUSE_PREEMPTION					1          //1使用抢占式内核  0使用协程 基本是1 多优先级任务可以抢占地优先级任务
#define configUSE_PORT_OPTIMISED_TASK_SELECTION	1          //启用特殊方法选择下一个要运行的任务 一般是硬件计算前导零指令

/***************************************************************************************************************/
/*                                FreeRTOS与内存申请有关配置选项                                                */
/***************************************************************************************************************/
#define configTOTAL_HEAP_SIZE					( ( size_t ) ( 22800 ) )   //系统所有总的堆大小 大了可能没法编译显示内存不足
//既然实现了内存管理，需要有一个内存池，在heap.c
//PRIVILEGED_DATA static uint8_t ucHeap[ configTOTAL_HEAP_SIZE ]; heap4

/***************************************************************************************************************/
/*                                FreeRTOS与钩子函数有关的配置选项                                              */
/***************************************************************************************************************/
#define configUSE_IDLE_HOOK						0  //1，使用空闲钩子；0，不使用
#define configUSE_TICK_HOOK						0  //1，使用时间片钩子；0，不使用

/***************************************************************************************************************/
/*                                FreeRTOS与运行时间和任务状态收集有关的配置选项                                 */
/***************************************************************************************************************/
#define configUSE_TRACE_FACILITY				1  //为1启用可视化跟踪调试



#define configCPU_CLOCK_HZ						( SystemCoreClock )        //CPU频率
#define configTICK_RATE_HZ						( ( TickType_t ) 1000 )    //时钟节拍频率，这里设置为1000，周期就是1ms
#define configMAX_PRIORITIES					( 5 )                      //可使用的最大优先级
#define configMINIMAL_STACK_SIZE				( ( unsigned short ) 130 ) //空闲任务使用的堆栈大小
	
#define configMAX_TASK_NAME_LEN					( 10 )                     //任务名字字符串长度
#define configUSE_16_BIT_TICKS					0                          //系统节拍计数器变量数据类型，1表示为16位无符号整形，0表示为32位无符号整形
#define configIDLE_SHOULD_YIELD					1                          //为1时空闲任务放弃CPU使用权给其他同优先级的用户任务
                                                                           //freertos如果什么任务都没有，就会执行空闲任务，对他没什么要求，应用任务和
#define configUSE_MUTEXES						1                          //为1时使用互斥信号量
#define configQUEUE_REGISTRY_SIZE				8                          //不为0时表示启用队列记录，具体的值是可以记录的队列和信号量最大数目。
#define configCHECK_FOR_STACK_OVERFLOW			2                          //大于0时启用堆栈溢出检测功能，如果使用此功能用户必须提供一个栈溢出钩子函数，
                                                                           //可以为1/2 有两种栈溢出检测方法（做产品要关掉）
#define configUSE_RECURSIVE_MUTEXES				1                          //为1时使用递归互斥信号量
#define configUSE_MALLOC_FAILED_HOOK			1                          //1 使用内存申请失败钩子函数
#define configUSE_APPLICATION_TASK_TAG			0                          //用户任务标签
#define configUSE_COUNTING_SEMAPHORES			1                          //为1时使用计数信号量
#define configGENERATE_RUN_TIME_STATS			0

/* Co-routine definitions. */
/***************************************************************************************************************/
/*                                FreeRTOS与协程有关的配置选项                                                  */
/***************************************************************************************************************/
#define configUSE_CO_ROUTINES 		0                 //为1时启用协程，启用协程以后必须添加文件croutine.c
#define configMAX_CO_ROUTINE_PRIORITIES ( 2 )         //协程的有效优先级数目

/* Software timer definitions. */
/***************************************************************************************************************/
/*                                FreeRTOS与软件定时器有关的配置选项                                            */
/***************************************************************************************************************/
#define configUSE_TIMERS				1             //为1时启用软件定时器
#define configTIMER_TASK_PRIORITY		( 2 )         //软件定时器优先级 //(configMAX_PRIORITIES-1)
#define configTIMER_QUEUE_LENGTH		5             //软件定时器队列长度
#define configTIMER_TASK_STACK_DEPTH	( configMINIMAL_STACK_SIZE * 2 )  //软件定时器任务堆栈大小

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */

/*INCLUDE开始的宏
比如当宏INCLUDE_vTaskPrioritySet设置为0的时候，表示不能使用函数vTaskPrioritySet()
#if ( INCLUDE_vTaskPrioritySet == 1 )  在task.c中

    void vTaskPrioritySet( TaskHandle_t xTask,
                           UBaseType_t uxNewPriority )
{.....................
FreeRTOS中的裁剪和配置就是用这种条件编译的方法来实现的，节省空间，根据使用的MCU来调制系统消耗
如果要使用函数，就要将对应的宏定义为1
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
/*                                FreeRTOS与中断有关的配置选项                                                  */
/***************************************************************************************************************/

#ifdef __NVIC_PRIO_BITS
	/* __BVIC_PRIO_BITS will be specified when CMSIS is being used. */
	#define configPRIO_BITS       		__NVIC_PRIO_BITS
#else
	#define configPRIO_BITS       		6        /* 63 priority levels */
#endif

/* The lowest interrupt priority that can be used in a call to a "set priority"
function. */
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY			0x3f           //63 中断最低优先级

/* The highest interrupt priority that can be used by any interrupt service
routine that makes calls to interrupt safe FreeRTOS API functions.  DO NOT CALL
INTERRUPT SAFE FREERTOS API FUNCTIONS FROM ANY INTERRUPT THAT HAS A HIGHER
PRIORITY THAN THIS! (higher priorities are lower numeric values. */
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY	5              //系统可管理的最高中断优先级

/* Interrupt priorities used by the kernel port layer itself.  These are generic
to all Cortex-M ports, and do not rely on any particular library functions. */
#define configKERNEL_INTERRUPT_PRIORITY 		( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) ) //XMC使用6位作为优先级，在高6位，需要左移2位去定义
//在FreeRTOS中PendSV和SysTick得中断优先级都是最低的
/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) ) //优先级级数小于5不归RTOS管理
//此宏设置好以后，高于此优先级的中断RTOS是不能禁止的，中端服务函数也不能调用FreeRTOS的API函数
	
/* Normal assert() semantics without relying on the provision of an assert.h
header file. */
#define configASSERT( x ) if( ( x ) == 0 ) { taskDISABLE_INTERRUPTS(); for( ;; ); }	
	
/* Definitions that map the FreeRTOS port interrupt handlers to their CMSIS
standard names.  WORKAROUND_PMU_CM001 is defined at the top of this file. */
/***************************************************************************************************************/
/*                                FreeRTOS与中断服务函数有关的配置选项                                          */
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

