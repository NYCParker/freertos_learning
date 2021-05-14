#include <XMC4400.h>
#include "Sys_Task.h"
#include "Var_Init.h"

#define AppTaskCreateTask_STACK_SIZE 512

/*Function declaration*/
static void LowPriority_Task(void* parameter);
static void MidPriority_Task(void* parameter);
static void HighPriority_Task(void* parameter);

int AppTaskCreate(void);
void AppTaskCreate_Task(void);

//任务控制块
static TaskHandle_t AppTaskCreate_Handle = NULL;
static TaskHandle_t LowPriority_Task_Handle = NULL;
static TaskHandle_t MidPriority_Task_Handle = NULL;
static TaskHandle_t HighPriority_Task_Handle = NULL;

//信号量控制块
SemaphoreHandle_t MuxSem_Handle =NULL;

static void LowPriority_Task(void* parameter)
{
	static uint32_t i;
	BaseType_t xReturn = pdPASS;       //创建信息返回值
	
	while(1)
	{
		printf("LowPriority_Task 获取信号量\n");
		xReturn = xSemaphoreTake(MuxSem_Handle,portMAX_DELAY);     //

		if ( xReturn == pdTRUE )
		{
			printf("LowPriority_Task Runing,占用中...\n");
		}

		for(i=0;i<2000000;i++)  //模拟低优先级任务占用信号量
		{
			taskYIELD();        //发起任务调度
		}
		
		printf("LowPriority_Task 释放信号量\n");
		xReturn = xSemaphoreGive( MuxSem_Handle );
			
		vTaskDelay(500);
	}
}

static void MidPriority_Task(void* parameter)
{
	while(1)
	{
		printf("MidPriority_Task Runing\n");
		vTaskDelay(500);
	}
}

static void HighPriority_Task(void* parameter)   //如果需要通过串口命令进行任务挂起，不能在串口中断中进行，设置一个串口任务
{
	BaseType_t xReturn = pdTRUE;
	
	while(1)
	{
		printf("HighPriority_Task 获取信号量\n");
		xReturn = xSemaphoreTake(MuxSem_Handle,portMAX_DELAY);
		if (pdTRUE == xReturn)
		{
			printf("HighPriority_Task Runing\n");
		}
		xReturn = xSemaphoreGive( MuxSem_Handle );
		
		vTaskDelay(500);
	}
}

void AppTaskCreate_Task(void)
{
	BaseType_t xReturn = pdPASS;
	
	taskENTER_CRITICAL();
	
	MuxSem_Handle = xSemaphoreCreateMutex();    //创建计数信号量
	
	if (NULL != MuxSem_Handle)
	{
		printf("BinarySem_Handle 互斥信号量创建成功!\r\n");
	}
	xReturn = xSemaphoreGive( MuxSem_Handle ); //创建出来是空的，先释放一个
	
	xReturn = xTaskCreate((TaskFunction_t )LowPriority_Task, 
						 (const char* )"LowPriority_Task",
						 (uint16_t )512, 
						 (void* )NULL,
						 (UBaseType_t )2, 
						 (TaskHandle_t* )&LowPriority_Task_Handle);

	if(xReturn == pdPASS)    
	{
		printf("创建LowPriority_Task任务成功\n");      
	}
	
	xReturn = xTaskCreate((TaskFunction_t )MidPriority_Task, 
						 (const char* )"MidPriority_Task",
						 (uint16_t )512, 
						 (void* )NULL,
						 (UBaseType_t )3, 
						 (TaskHandle_t* )&MidPriority_Task_Handle);

	if(xReturn == pdPASS)    
	{
		printf("创建MidPriority_Task任务成功\n");      
	}	
	
	xReturn = xTaskCreate((TaskFunction_t )HighPriority_Task, 
						 (const char* )"HighPriority_Task",
						 (uint16_t )512, 
						 (void* )NULL,
						 (UBaseType_t )4, 
						 (TaskHandle_t* )&HighPriority_Task_Handle);

	if(xReturn == pdPASS)    
	{
		printf("创建HighPriority_Task任务成功\n");      
	}
	vTaskDelete(AppTaskCreate_Handle);
	
    taskEXIT_CRITICAL();
}										

int AppTaskCreate(void)
{
	BaseType_t xReturn = pdPASS;
	
	printf("互斥量实验\n");
	
	xReturn = xTaskCreate((TaskFunction_t )AppTaskCreate_Task, 
						 (const char* )"AppTaskCreate_Task",
						 (uint16_t )AppTaskCreateTask_STACK_SIZE, 
						 (void* )NULL,
						 (UBaseType_t )1, 
						 (TaskHandle_t* )&AppTaskCreate_Handle);
	
	if(xReturn == pdPASS)    
	{
		printf("开始创建任务\n");
		vTaskStartScheduler();          
	}
	else
	{
		return -1;
	}
	
	while(1);
}

