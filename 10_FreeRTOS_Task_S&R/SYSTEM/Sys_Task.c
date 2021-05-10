#include <XMC4400.h>
#include "Sys_Task.h"

#define AppTaskCreateTask_STACK_SIZE 512
#define LED1_Task_STACK_SIZE 512
#define LED2_Task_STACK_SIZE 512
#define UCMD_Task_STACK_SIZE 512

/*Function declaration*/
static void LED1_Task(void* parameter);
static void LED2_Task(void* parameter);
static void UCMD_Task(void* parameter);

int AppTaskCreate(void);
void AppTaskCreate_Task(void);

//任务控制块
static TaskHandle_t AppTaskCreate_Handle = NULL;
static TaskHandle_t LED1_Task_Handle = NULL;
static TaskHandle_t LED2_Task_Handle = NULL;
static TaskHandle_t UCMD_Task_Handle = NULL;

static void LED1_Task(void* parameter)
{
	while(1)
	{
		XMC_GPIO_ToggleOutput(GPIO_LED1);
		vTaskDelay(1000);
	}
}

static void LED2_Task(void* parameter)
{
	while(1)
	{
		XMC_GPIO_ToggleOutput(GPIO_LED2);
		vTaskDelay(500);
	}
}

static void UCMD_Task(void* parameter)   //不建议没加阻塞，要么设置优先级最低，要么加点阻塞，比如50ms扫描一次Task_Cmd信号
{
	while(1)
	{
		switch(Task_Cmd)
		{      
			case 1:                                 
			{
				vTaskSuspend(LED1_Task_Handle);
				Task_Cmd = 0;   //清除本次指令
				break;
			}
			case 2:                                  
			{
				vTaskResume(LED1_Task_Handle);
				Task_Cmd = 0;   //清除本次指令
				break;
			}		
			default:
			break;
		}
		
		vTaskDelay(50);
	}
}

int AppTaskCreate(void)
{
	BaseType_t xReturn = pdPASS;
	
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

void AppTaskCreate_Task(void)
{
	BaseType_t xReturn = pdPASS;
	
	taskENTER_CRITICAL();
	
	xReturn = xTaskCreate((TaskFunction_t )LED1_Task, 
						 (const char* )"LED1_Task",
						 (uint16_t )LED1_Task_STACK_SIZE, 
						 (void* )NULL,
						 (UBaseType_t )2, 
						 (TaskHandle_t* )&LED1_Task_Handle);

	if(xReturn == pdPASS)    
	{
		printf("创建LED1_Task任务成功\n");      
	}
	
	xReturn = xTaskCreate((TaskFunction_t )LED2_Task, 
						 (const char* )"LED2_Task",
						 (uint16_t )LED2_Task_STACK_SIZE, 
						 (void* )NULL,
						 (UBaseType_t )3, 
						 (TaskHandle_t* )&LED2_Task_Handle);

	if(xReturn == pdPASS)    
	{
		printf("创建LED2_Task任务成功\n");      
	}	
	
	xReturn = xTaskCreate((TaskFunction_t )UCMD_Task, 
						 (const char* )"UCMD_Task",
						 (uint16_t )UCMD_Task_STACK_SIZE, 
						 (void* )NULL,
						 (UBaseType_t )4, 
						 (TaskHandle_t* )&UCMD_Task_Handle);

	if(xReturn == pdPASS)    
	{
		printf("创建UCMD_Task任务成功\n");      
	}
	
	vTaskDelete(AppTaskCreate_Handle);
	
    taskEXIT_CRITICAL();
}										



