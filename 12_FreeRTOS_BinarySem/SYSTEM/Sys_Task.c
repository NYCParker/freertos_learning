#include <XMC4400.h>
#include "Sys_Task.h"
#include "Var_Init.h"

#define AppTaskCreateTask_STACK_SIZE 512

/*Function declaration*/
static void BinarySem_Send_Task(void* parameter);
static void BinarySem_Receive_Task(void* parameter);
static void UCMD_Task(void* parameter);

int AppTaskCreate(void);
void AppTaskCreate_Task(void);

//任务控制块
static TaskHandle_t AppTaskCreate_Handle;
static TaskHandle_t BinarySem_Send_Task_Handle;
static TaskHandle_t BinarySem_Receive_Task_Handle;
static TaskHandle_t UCMD_Task_Handle = NULL;

//二值信号量控制块
SemaphoreHandle_t BinarySem_Handle = NULL;


static void BinarySem_Send_Task(void* parameter)
{
	BaseType_t xReturn = pdPASS;       //创建信息返回值
	
	while(1)
	{
		if(Task_Cmd == 1)
		{
			xReturn = xSemaphoreGive(BinarySem_Handle);
			
			if ( xReturn == pdTRUE )
			{
				printf("BinarySem_Handle 二值信号量释放成功!\r\n");
			}
			else
			{
				printf("BinarySem_Handle 二值信号量释放失败!\r\n");
			}
			Task_Cmd = 0;
		}

		vTaskDelay(50);
	}
}

static void BinarySem_Receive_Task(void* parameter)
{
	BaseType_t xReturn = pdTRUE;
	
	while(1)
	{
		xReturn = xSemaphoreTake(BinarySem_Handle , portMAX_DELAY);
		if (pdTRUE == xReturn)
		{
			printf("BinarySem_Handle 二值信号量获取成功!\n");
		}
	}
}

static void UCMD_Task(void* parameter)   //如果需要通过串口命令进行任务挂起，不能在串口中断中进行，设置一个串口任务
{
	while(1)
	{
//		switch(Task_Cmd)
//		{      
//			case 1:                                 
//			{
//				
//				Task_Cmd = 0;   //清除本次指令
//				break;
//			}
//			case 2:                                  
//			{
//				
//				Task_Cmd = 0;   //清除本次指令
//				break;
//			}		
//			default:
//			break;
//		}
		vTaskDelay(50);
	}
}

void AppTaskCreate_Task(void)
{
	BaseType_t xReturn = pdPASS;
	
	taskENTER_CRITICAL();
	
	BinarySem_Handle = xSemaphoreCreateBinary();    //创建二值信号量
	if (NULL != BinarySem_Handle)
	{
		printf("BinarySem_Handle 二值信号量创建成功!\r\n");
	}
	
	xReturn = xTaskCreate((TaskFunction_t )BinarySem_Send_Task, 
						 (const char* )"BinarySem_Send_Task",
						 (uint16_t )512, 
						 (void* )NULL,
						 (UBaseType_t )2, 
						 (TaskHandle_t* )&BinarySem_Send_Task_Handle);

	if(xReturn == pdPASS)    
	{
		printf("创建BinarySem_Send_Task任务成功\n");      
	}
	
	xReturn = xTaskCreate((TaskFunction_t )BinarySem_Receive_Task, 
						 (const char* )"BinarySem_Receive_Task",
						 (uint16_t )512, 
						 (void* )NULL,
						 (UBaseType_t )3, 
						 (TaskHandle_t* )&BinarySem_Receive_Task_Handle);

	if(xReturn == pdPASS)    
	{
		printf("创建BinarySem_Receive_Task任务成功\n");      
	}	
	
	xReturn = xTaskCreate((TaskFunction_t )UCMD_Task, 
						 (const char* )"UCMD_Task",
						 (uint16_t )512, 
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

int AppTaskCreate(void)
{
	BaseType_t xReturn = pdPASS;
	
	printf("二值信号量实验\n");
	
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

