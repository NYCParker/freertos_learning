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

//������ƿ�
static TaskHandle_t AppTaskCreate_Handle;
static TaskHandle_t Queue_Send_Task_Handle;
static TaskHandle_t Queue_Receive_Task_Handle;
static TaskHandle_t UCMD_Task_Handle = NULL;

//���п��ƿ�
QueueHandle_t Test_Queue = NULL;


static void Queue_Send_Task(void* parameter)
{
	BaseType_t xReturn = pdPASS;       //������Ϣ����ֵ
	uint32_t Send_Data1 = 1;           //һ����Ϣ4���ֽ�
	uint32_t Send_Data2 = 2;
	
	while(1)
	{
		if(Task_Cmd == 1)
		{
			xReturn = xQueueSend(Test_Queue,&Send_Data1,0);
			if (pdPASS == xReturn)
			{
				printf("��Ϣ Send_Data1 ���ͳɹ�!\n");
			}
			Task_Cmd = 0;
		}
		
		if(Task_Cmd == 2)
		{
			xReturn = xQueueSend(Test_Queue,&Send_Data2,0);
			if (pdPASS == xReturn)
			{
				printf("��Ϣ Send_Data2 ���ͳɹ�!\n");
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
			printf("���ν��յ���������%d\n",r_queue);
		}
		else
		{
			printf("���ݽ��ճ���,�������: 0x%lx\n",xReturn);
		}
		vTaskDelay(50);
	}
}

static void UCMD_Task(void* parameter)   //�����Ҫͨ�������������������𣬲����ڴ����ж��н���
{
	while(1)
	{
//		switch(Task_Cmd)
//		{      
//			case 1:                                 
//			{
//				
//				Task_Cmd = 0;   //�������ָ��
//				break;
//			}
//			case 2:                                  
//			{
//				
//				Task_Cmd = 0;   //�������ָ��
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
		printf("����Queue_Send_Task����ɹ�\n");      
	}
	
	xReturn = xTaskCreate((TaskFunction_t )Queue_Receive_Task, 
						 (const char* )"Queue_Receive_Task",
						 (uint16_t )512, 
						 (void* )NULL,
						 (UBaseType_t )3, 
						 (TaskHandle_t* )&Queue_Receive_Task_Handle);

	if(xReturn == pdPASS)    
	{
		printf("����Queue_Receive_Task����ɹ�\n");      
	}	
	
	xReturn = xTaskCreate((TaskFunction_t )UCMD_Task, 
						 (const char* )"UCMD_Task",
						 (uint16_t )512, 
						 (void* )NULL,
						 (UBaseType_t )4, 
						 (TaskHandle_t* )&UCMD_Task_Handle);

	if(xReturn == pdPASS)    
	{
		printf("����UCMD_Task����ɹ�\n");      
	}
	vTaskDelete(AppTaskCreate_Handle);
	
    taskEXIT_CRITICAL();
}										

int AppTaskCreate(void)
{
	BaseType_t xReturn = pdPASS;
	
	printf("��Ϣ����ʵ��\n");
	printf("���ڷ���ָ�Send��������з�����Ϣ\n");
	printf("Receive����Ӷ��н�����Ϣ����ʾ\n");
	
	xReturn = xTaskCreate((TaskFunction_t )AppTaskCreate_Task, 
						 (const char* )"AppTaskCreate_Task",
						 (uint16_t )AppTaskCreateTask_STACK_SIZE, 
						 (void* )NULL,
						 (UBaseType_t )1, 
						 (TaskHandle_t* )&AppTaskCreate_Handle);
	
	if(xReturn == pdPASS)    
	{
		printf("��ʼ��������\n");
		vTaskStartScheduler();          
	}
	else
	{
		return -1;
	}
	
	while(1);
}

