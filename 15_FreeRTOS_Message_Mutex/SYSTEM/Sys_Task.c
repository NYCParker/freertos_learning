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

//������ƿ�
static TaskHandle_t AppTaskCreate_Handle = NULL;
static TaskHandle_t LowPriority_Task_Handle = NULL;
static TaskHandle_t MidPriority_Task_Handle = NULL;
static TaskHandle_t HighPriority_Task_Handle = NULL;

//�ź������ƿ�
SemaphoreHandle_t MuxSem_Handle =NULL;

static void LowPriority_Task(void* parameter)
{
	static uint32_t i;
	BaseType_t xReturn = pdPASS;       //������Ϣ����ֵ
	
	while(1)
	{
		printf("LowPriority_Task ��ȡ�ź���\n");
		xReturn = xSemaphoreTake(MuxSem_Handle,portMAX_DELAY);     //

		if ( xReturn == pdTRUE )
		{
			printf("LowPriority_Task Runing,ռ����...\n");
		}

		for(i=0;i<2000000;i++)  //ģ������ȼ�����ռ���ź���
		{
			taskYIELD();        //�����������
		}
		
		printf("LowPriority_Task �ͷ��ź���\n");
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

static void HighPriority_Task(void* parameter)   //�����Ҫͨ�������������������𣬲����ڴ����ж��н��У�����һ����������
{
	BaseType_t xReturn = pdTRUE;
	
	while(1)
	{
		printf("HighPriority_Task ��ȡ�ź���\n");
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
	
	MuxSem_Handle = xSemaphoreCreateMutex();    //���������ź���
	
	if (NULL != MuxSem_Handle)
	{
		printf("BinarySem_Handle �����ź��������ɹ�!\r\n");
	}
	xReturn = xSemaphoreGive( MuxSem_Handle ); //���������ǿյģ����ͷ�һ��
	
	xReturn = xTaskCreate((TaskFunction_t )LowPriority_Task, 
						 (const char* )"LowPriority_Task",
						 (uint16_t )512, 
						 (void* )NULL,
						 (UBaseType_t )2, 
						 (TaskHandle_t* )&LowPriority_Task_Handle);

	if(xReturn == pdPASS)    
	{
		printf("����LowPriority_Task����ɹ�\n");      
	}
	
	xReturn = xTaskCreate((TaskFunction_t )MidPriority_Task, 
						 (const char* )"MidPriority_Task",
						 (uint16_t )512, 
						 (void* )NULL,
						 (UBaseType_t )3, 
						 (TaskHandle_t* )&MidPriority_Task_Handle);

	if(xReturn == pdPASS)    
	{
		printf("����MidPriority_Task����ɹ�\n");      
	}	
	
	xReturn = xTaskCreate((TaskFunction_t )HighPriority_Task, 
						 (const char* )"HighPriority_Task",
						 (uint16_t )512, 
						 (void* )NULL,
						 (UBaseType_t )4, 
						 (TaskHandle_t* )&HighPriority_Task_Handle);

	if(xReturn == pdPASS)    
	{
		printf("����HighPriority_Task����ɹ�\n");      
	}
	vTaskDelete(AppTaskCreate_Handle);
	
    taskEXIT_CRITICAL();
}										

int AppTaskCreate(void)
{
	BaseType_t xReturn = pdPASS;
	
	printf("������ʵ��\n");
	
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

