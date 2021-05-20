#include <XMC4400.h>
#include "Sys_Task.h"
#include "Var_Init.h"

#define AppTaskCreateTask_STACK_SIZE 512
#define QUEUE_LEN 4
#define QUEUE_SIZE 4
/*Function declaration*/
static void Queue_Send_Task(void* parameter);
static void Queue_Receive_Task(void* parameter);
static void UCMD_Task(void* parameter);

int AppTaskCreate(void);
void AppTaskCreate_Task(void);

//任务控制块
static TaskHandle_t AppTaskCreate_Handle;
static TaskHandle_t Queue_Send_Task_Handle;
static TaskHandle_t Queue_Receive_Task_Handle;
static TaskHandle_t UCMD_Task_Handle = NULL;

//队列控制块
QueueHandle_t Test_Queue = NULL;


static void Queue_Send_Task(void* parameter)
{
	BaseType_t xReturn = pdPASS;       //创建信息返回值
	uint32_t Send_Data1 = 1;           //一个消息4个字节
	uint32_t Send_Data2 = 2;
	
	while(1)
	{
		if(Task_Cmd == 1)
		{
			xReturn = xQueueSend(Test_Queue,&Send_Data1,0);
			if (pdPASS == xReturn)
			{
				printf("消息 Send_Data1 发送成功!\n");
			}
			Task_Cmd = 0;
		}
		
		if(Task_Cmd == 2)
		{
			xReturn = xQueueSend(Test_Queue,&Send_Data2,0);
			if (pdPASS == xReturn)
			{
				printf("消息 Send_Data2 发送成功!\n");
			}
			Task_Cmd = 0;
		}

		vTaskDelay(50);
	}
}

static void Queue_Receive_Task(void* parameter)
{
	BaseType_t xReturn = pdTRUE;
	uint32_t r_queue;
	
	while(1)
	{
		xReturn = xQueueReceive(Test_Queue,&r_queue, portMAX_DELAY);
		if (pdTRUE == xReturn)
		{
			printf("本次接收到的数据是%d\n",r_queue);
		}
		else
		{
			printf("数据接收出错,错误代码: 0x%lx\n",xReturn);
		}
		vTaskDelay(50);
	}
}

static void UCMD_Task(void* parameter)   //如果需要通过串口命令进行任务挂起，不能在串口中断中进行
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
	
	Test_Queue = xQueueCreate((UBaseType_t ) QUEUE_LEN, (UBaseType_t ) QUEUE_SIZE);
	
	xReturn = xTaskCreate((TaskFunction_t )Queue_Send_Task, 
						 (const char* )"Queue_Send_Task",
						 (uint16_t )512, 
						 (void* )NULL,
						 (UBaseType_t )2, 
						 (TaskHandle_t* )&Queue_Send_Task_Handle);

	if(xReturn == pdPASS)    
	{
		printf("创建Queue_Send_Task任务成功\n");      
	}
	
	xReturn = xTaskCreate((TaskFunction_t )Queue_Receive_Task, 
						 (const char* )"Queue_Receive_Task",
						 (uint16_t )512, 
						 (void* )NULL,
						 (UBaseType_t )3, 
						 (TaskHandle_t* )&Queue_Receive_Task_Handle);

	if(xReturn == pdPASS)    
	{
		printf("创建Queue_Receive_Task任务成功\n");      
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

