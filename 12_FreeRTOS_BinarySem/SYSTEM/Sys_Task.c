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

//������ƿ�
static TaskHandle_t AppTaskCreate_Handle;
static TaskHandle_t BinarySem_Send_Task_Handle;
static TaskHandle_t BinarySem_Receive_Task_Handle;
static TaskHandle_t UCMD_Task_Handle = NULL;

//��ֵ�ź������ƿ�
SemaphoreHandle_t BinarySem_Handle = NULL;


static void BinarySem_Send_Task(void* parameter)
{
	BaseType_t xReturn = pdPASS;       //������Ϣ����ֵ
	
	while(1)
	{
		if(Task_Cmd == 1)
		{
			xReturn = xSemaphoreGive(BinarySem_Handle);
			
			if ( xReturn == pdTRUE )
			{
				printf("BinarySem_Handle ��ֵ�ź����ͷųɹ�!\r\n");
			}
			else
			{
				printf("BinarySem_Handle ��ֵ�ź����ͷ�ʧ��!\r\n");
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
			printf("BinarySem_Handle ��ֵ�ź�����ȡ�ɹ�!\n");
		}
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
	
	BinarySem_Handle = xSemaphoreCreateBinary();    //������ֵ�ź���
	if (NULL != BinarySem_Handle)
	{
		printf("BinarySem_Handle ��ֵ�ź��������ɹ�!\r\n");
	}
	
	xReturn = xTaskCreate((TaskFunction_t )BinarySem_Send_Task, 
						 (const char* )"BinarySem_Send_Task",
						 (uint16_t )512, 
						 (void* )NULL,
						 (UBaseType_t )2, 
						 (TaskHandle_t* )&BinarySem_Send_Task_Handle);

	if(xReturn == pdPASS)    
	{
		printf("����BinarySem_Send_Task����ɹ�\n");      
	}
	
	xReturn = xTaskCreate((TaskFunction_t )BinarySem_Receive_Task, 
						 (const char* )"BinarySem_Receive_Task",
						 (uint16_t )512, 
						 (void* )NULL,
						 (UBaseType_t )3, 
						 (TaskHandle_t* )&BinarySem_Receive_Task_Handle);

	if(xReturn == pdPASS)    
	{
		printf("����BinarySem_Receive_Task����ɹ�\n");      
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
	
	printf("��ֵ�ź���ʵ��\n");
	
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

