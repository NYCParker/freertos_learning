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

//������ƿ�
static TaskHandle_t AppTaskCreate_Handle;
static TaskHandle_t CountSem_Send_Task_Handle;
static TaskHandle_t CountSem_Receive_Task_Handle;
static TaskHandle_t UCMD_Task_Handle = NULL;

//�ź������ƿ�
SemaphoreHandle_t CountSem_Handle = NULL;


static void CountSem_Send_Task(void* parameter)
{
	BaseType_t xReturn = pdTRUE;       //������Ϣ����ֵ
	
	while(1)
	{
		if(Task_Cmd == 1)
		{
			xReturn = xSemaphoreGive(CountSem_Handle);   //���������ź���
			
			if ( xReturn == pdTRUE )
			{
				printf("�ͷ�һ��ͣ��λ\r\n");
			}
			else
			{
				printf("��ͣ��Ϊ�����ͷ�!\r\n");
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
			xReturn = xSemaphoreTake(CountSem_Handle , 0);   //���������ź���
			
			if ( xReturn == pdTRUE )
			{
				printf("��ȡһ��ͣ��λ\r\n");
			}
			else
			{
				printf("��ͣ��λ���Ի�ȡ!\r\n");
			}
			
			Task_Cmd = 0;
		}
		
		vTaskDelay(50);
	}
}

static void UCMD_Task(void* parameter)   //�����Ҫͨ�������������������𣬲����ڴ����ж��н��У�����һ����������
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
	
	CountSem_Handle = xSemaphoreCreateCounting(5,5);    //���������ź���
	
	if (NULL != CountSem_Handle)
	{
		printf("CountSem_Handle �����ź��������ɹ�!\r\n");
	}
	
	xReturn = xTaskCreate((TaskFunction_t )CountSem_Send_Task, 
						 (const char* )"CountSem_Send_Task",
						 (uint16_t )512, 
						 (void* )NULL,
						 (UBaseType_t )2, 
						 (TaskHandle_t* )&CountSem_Send_Task_Handle);

	if(xReturn == pdPASS)    
	{
		printf("����CountSem_Send_Task����ɹ�\n");      
	}
	
	xReturn = xTaskCreate((TaskFunction_t )CountSem_Receive_Task, 
						 (const char* )"CountSem_Receive_Task",
						 (uint16_t )512, 
						 (void* )NULL,
						 (UBaseType_t )3, 
						 (TaskHandle_t* )&CountSem_Receive_Task_Handle);

	if(xReturn == pdPASS)    
	{
		printf("����CountSem_Receive_Task����ɹ�\n");      
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

