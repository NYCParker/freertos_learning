#include <XMC4400.h>
#include "Sys_Task.h"
#include "Var_Init.h"
#include "event_groups.h"

#define EVENT_1 (0x01 << 0)//�����¼������λ 0
#define EVENT_2 (0x01 << 1)//�����¼������λ 1

#define AppTaskCreateTask_STACK_SIZE 512
#define UCMDTask_STACK_SIZE 512    //���ڽ����¼�����
#define LEDTask_STACK_SIZE 512     //LED�ȵ����ڷֱ��յ�1��2ָ���¼��� �Żᷭת

/*Function declaration*/
static void UCMD_Task(void* parameter);
static void LED_Task(void* parameter);

int AppTaskCreate(void);
void AppTaskCreate_Task(void);

//������ƿ�
static TaskHandle_t AppTaskCreate_Handle = NULL;
static TaskHandle_t UCMD_Task_Handle = NULL;
static TaskHandle_t LED_Task_Handle = NULL;

//�¼����ƿ�
static EventGroupHandle_t Event_Handle =NULL;

static void UCMD_Task(void* parameter)
{
	while(1)
	{
		if(Task_Cmd == 1)
		{
			printf("�¼�1����\n");
			xEventGroupSetBits(Event_Handle,EVENT_1);
			Task_Cmd = 0;
		}
		if(Task_Cmd == 2)
		{
			printf("�¼�2����\n");
			xEventGroupSetBits(Event_Handle,EVENT_2);
			Task_Cmd = 0;
		}		
	}
	vTaskDelay(50);
}

static void LED_Task(void* parameter)
{
	EventBits_t r_event;  //ע������ ����һ���¼����ձ���
	
	while(1)
	{
		r_event = xEventGroupWaitBits(Event_Handle, /* �¼������� */
										EVENT_1|EVENT_2,/* �����������Ȥ���¼� */
										pdTRUE, /* �˳�ʱ����¼�λ */
										pdTRUE, /* �������Ȥ�������¼� */
										portMAX_DELAY);/* ָ����ʱ�¼�,һֱ�� */
		if ((r_event & (EVENT_1|EVENT_2)) == (EVENT_1|EVENT_2)) 
		{
			printf("1��2�¼������� ��ת��\n");
			XMC_GPIO_ToggleOutput(GPIO_LED1);
		}
	}
}

void AppTaskCreate_Task(void)
{
	BaseType_t xReturn = pdPASS;
	
	taskENTER_CRITICAL();
	
	Event_Handle = xEventGroupCreate();    //���������ź���
	
	if (NULL != Event_Handle)
	{
		printf("Event_Handle �¼������ɹ�!\r\n");
	}
	
	xReturn = xTaskCreate((TaskFunction_t )UCMD_Task, 
						 (const char* )"UCMD_Task",
						 (uint16_t )UCMDTask_STACK_SIZE, 
						 (void* )NULL,
						 (UBaseType_t )2, 
						 (TaskHandle_t* )&UCMD_Task_Handle);

	if(xReturn == pdPASS)    
	{
		printf("����UCMD_Task����ɹ�\n");      
	}
	
	xReturn = xTaskCreate((TaskFunction_t )LED_Task, 
						 (const char* )"LED_Task",
						 (uint16_t )LEDTask_STACK_SIZE, 
						 (void* )NULL,
						 (UBaseType_t )3, 
						 (TaskHandle_t* )&LED_Task_Handle);

	if(xReturn == pdPASS)    
	{
		printf("����LED_Task����ɹ�\n");      
	}	

	vTaskDelete(AppTaskCreate_Handle);
	
    taskEXIT_CRITICAL();
}										

int AppTaskCreate(void)
{
	BaseType_t xReturn = pdPASS;
	
	printf("�¼�ʵ��\n");
	
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

