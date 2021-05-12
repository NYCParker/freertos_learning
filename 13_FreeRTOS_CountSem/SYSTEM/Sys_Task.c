#include <XMC4400.h>
#include "Sys_Task.h"
#include "Var_Init.h"

#define AppTaskCreateTask_STACK_SIZE 512

/*Function declaration*/
static void CountSem_Send_Task(void* parameter);
static void CountSem_Receive_Task(void* parameter);
static void UCMD_Task(void* parameter);

int AppTaskCreate(void);
void AppTaskCreate_Task(void);

//任务控制块
static TaskHandle_t AppTaskCreate_Handle;
static TaskHandle_t CountSem_Send_Task_Handle;
static TaskHandle_t CountSem_Receive_Task_Handle;
static TaskHandle_t UCMD_Task_Handle = NULL;

//信号量控制块
SemaphoreHandle_t CountSem_Handle = NULL;


static void CountSem_Send_Task(void* parameter)
{
	BaseType_t xReturn = pdTRUE;       //创建信息返回值
	
	while(1)
	{
		if(Task_Cmd == 1)
		{
			xReturn = xSemaphoreGive(CountSem_Handle);   //给出计数信号量
			
			if ( xReturn == pdTRUE )
			{
				printf("释放一个停车位\r\n");
			}
			else
			{
				printf("无停车为可以释放!\r\n");
			}
			
			Task_Cmd = 0;
		}

		vTaskDelay(50);
	}
}

static void CountSem_Receive_Task(void* parameter)
{
	BaseType_t xReturn = pdTRUE;
	uint32_t r_queue;
	
	while(1)
	{
		if(Task_Cmd == 2)
		{
			xReturn = xSemaphoreTake(CountSem_Handle , 0);   //给出计数信号量
			
			if ( xReturn == pdTRUE )
			{
				printf("获取一个停车位\r\n");
			}
			else
			{
				printf("无停车位可以获取!\r\n");
			}
			
			Task_Cmd = 0;
		}
		
		vTaskDelay(50);
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
	
	CountSem_Handle = xSemaphoreCreateCounting(5,5);    //创建计数信号量
	
	if (NULL != CountSem_Handle)
	{
		printf("CountSem_Handle 计数信号量创建成功!\r\n");
	}
	
	xReturn = xTaskCreate((TaskFunction_t )CountSem_Send_Task, 
						 (const char* )"CountSem_Send_Task",
						 (uint16_t )512, 
						 (void* )NULL,
						 (UBaseType_t )2, 
						 (TaskHandle_t* )&CountSem_Send_Task_Handle);

	if(xReturn == pdPASS)    
	{
		printf("创建CountSem_Send_Task任务成功\n");      
	}
	
	xReturn = xTaskCreate((TaskFunction_t )CountSem_Receive_Task, 
						 (const char* )"CountSem_Receive_Task",
						 (uint16_t )512, 
						 (void* )NULL,
						 (UBaseType_t )3, 
						 (TaskHandle_t* )&CountSem_Receive_Task_Handle);

	if(xReturn == pdPASS)    
	{
		printf("创建CountSem_Receive_Task任务成功\n");      
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
	
	printf("消息队列实验\n");
	printf("串口发送指令，Send任务向队列发送消息\n");
	printf("Receive任务从队列接收消息并显示\n");
	
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

