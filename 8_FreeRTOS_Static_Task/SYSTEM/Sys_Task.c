#include <XMC4400.h>
#include "Sys_Task.h"

/*Function declaration*/
static void LED_Task(void* parameter);
void AppTaskCreate(void);
void AppTaskCreate_Task(void);


//任务栈
static StackType_t Idle_Task_Stack[configMINIMAL_STACK_SIZE];     //空闲任务堆栈
static StackType_t Timer_Task_Stack[configTIMER_TASK_STACK_DEPTH];//定时器堆栈
static StackType_t AppTaskCreate_Stack[configMINIMAL_STACK_SIZE];
static StackType_t LED_Task_Stack[configMINIMAL_STACK_SIZE];

//任务控制块
static StaticTask_t Idle_Task_TCB;
static StaticTask_t Timer_Task_TCB;
static StaticTask_t AppTaskCreate_TCB;
static StaticTask_t LED_Task_TCB;

//任务句柄
static TaskHandle_t AppTaskCreate_Handle;
static TaskHandle_t LED_Task_Handle;

static void LED_Task(void* parameter)
{
	while(1)
	{
		XMC_GPIO_ToggleOutput(GPIO_LED1);
		vTaskDelay(500);
		XMC_GPIO_ToggleOutput(GPIO_LED2);
		vTaskDelay(500);
	}
}

void AppTaskCreate(void)
{
	AppTaskCreate_Handle = xTaskCreateStatic((TaskFunction_t)AppTaskCreate_Task,
											(const char* )"AppTaskCreate",
											(uint32_t )configMINIMAL_STACK_SIZE, 
											(void* )NULL, 
											(UBaseType_t )3, 
											(StackType_t* )AppTaskCreate_Stack, 
											(StaticTask_t* )&AppTaskCreate_TCB);

	if(AppTaskCreate_Handle != NULL)    
	{
		printf("开始创建任务\n");
		vTaskStartScheduler();          
	}
	while(1);
}

void AppTaskCreate_Task(void)
{
	taskENTER_CRITICAL();
	
	LED_Task_Handle = xTaskCreateStatic((TaskFunction_t )LED_Task, 
										(const char*)"LED_Task",
										(uint32_t)configMINIMAL_STACK_SIZE, 
										(void* )NULL, 
										(UBaseType_t)4, 
										(StackType_t*)LED_Task_Stack,
										(StaticTask_t*)&LED_Task_TCB);

										
	if (NULL != LED_Task_Handle)     printf("创建LED_Task任务成功\n");
	else                             printf("创建LED_Task任务失败\n");

	vTaskDelete(AppTaskCreate_Handle);
										
    taskEXIT_CRITICAL();
}										


void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,StackType_t **ppxIdleTaskStackBuffer,uint32_t *pulIdleTaskStackSize)
{
	*ppxIdleTaskTCBBuffer=&Idle_Task_TCB;            /* 任务控制块内存 */
	*ppxIdleTaskStackBuffer=Idle_Task_Stack;         /* 任务堆栈内存 */
	*pulIdleTaskStackSize=configMINIMAL_STACK_SIZE;  /* 任务堆栈大小 */
}

void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,StackType_t **ppxTimerTaskStackBuffer,uint32_t *pulTimerTaskStackSize)
{
	*ppxTimerTaskTCBBuffer=&Timer_Task_TCB;             /* 任务控制块内存 */
	*ppxTimerTaskStackBuffer=Timer_Task_Stack;          /* 任务堆栈内存 */
	*pulTimerTaskStackSize=configTIMER_TASK_STACK_DEPTH;/* 任务堆栈大小 */
}
