#include <XMC4400.h>
#include "Sys_Task.h"
#include "Var_Init.h"
#include "event_groups.h"

#define EVENT_1 (0x01 << 0)//设置事件掩码的位 0
#define EVENT_2 (0x01 << 1)//设置事件掩码的位 1

#define AppTaskCreateTask_STACK_SIZE 512
#define UCMDTask_STACK_SIZE 512    //串口接收事件任务
#define LEDTask_STACK_SIZE 512     //LED等到串口分别收到1和2指令事件后 才会翻转

/*Function declaration*/
static void UCMD_Task(void* parameter);
static void LED_Task(void* parameter);

int AppTaskCreate(void);
void AppTaskCreate_Task(void);

//任务控制块
static TaskHandle_t AppTaskCreate_Handle = NULL;
static TaskHandle_t UCMD_Task_Handle = NULL;
static TaskHandle_t LED_Task_Handle = NULL;

//事件控制块
static EventGroupHandle_t Event_Handle =NULL;

static void UCMD_Task(void* parameter)
{
	while(1)
	{
		if(Task_Cmd == 1)
		{
			printf("事件1发生\n");
			xEventGroupSetBits(Event_Handle,EVENT_1);
			Task_Cmd = 0;
		}
		if(Task_Cmd == 2)
		{
			printf("事件2发生\n");
			xEventGroupSetBits(Event_Handle,EVENT_2);
			Task_Cmd = 0;
		}		
	}
	vTaskDelay(50);
}

static void LED_Task(void* parameter)
{
	EventBits_t r_event;  //注意这里 定义一个事件接收变量
	
	while(1)
	{
		r_event = xEventGroupWaitBits(Event_Handle, /* 事件对象句柄 */
										EVENT_1|EVENT_2,/* 接收任务感兴趣的事件 */
										pdTRUE, /* 退出时清除事件位 */
										pdTRUE, /* 满足感兴趣的所有事件 */
										portMAX_DELAY);/* 指定超时事件,一直等 */
		if ((r_event & (EVENT_1|EVENT_2)) == (EVENT_1|EVENT_2)) 
		{
			printf("1，2事件都发生 翻转灯\n");
			XMC_GPIO_ToggleOutput(GPIO_LED1);
		}
	}
}

void AppTaskCreate_Task(void)
{
	BaseType_t xReturn = pdPASS;
	
	taskENTER_CRITICAL();
	
	Event_Handle = xEventGroupCreate();    //创建计数信号量
	
	if (NULL != Event_Handle)
	{
		printf("Event_Handle 事件创建成功!\r\n");
	}
	
	xReturn = xTaskCreate((TaskFunction_t )UCMD_Task, 
						 (const char* )"UCMD_Task",
						 (uint16_t )UCMDTask_STACK_SIZE, 
						 (void* )NULL,
						 (UBaseType_t )2, 
						 (TaskHandle_t* )&UCMD_Task_Handle);

	if(xReturn == pdPASS)    
	{
		printf("创建UCMD_Task任务成功\n");      
	}
	
	xReturn = xTaskCreate((TaskFunction_t )LED_Task, 
						 (const char* )"LED_Task",
						 (uint16_t )LEDTask_STACK_SIZE, 
						 (void* )NULL,
						 (UBaseType_t )3, 
						 (TaskHandle_t* )&LED_Task_Handle);

	if(xReturn == pdPASS)    
	{
		printf("创建LED_Task任务成功\n");      
	}	

	vTaskDelete(AppTaskCreate_Handle);
	
    taskEXIT_CRITICAL();
}										

int AppTaskCreate(void)
{
	BaseType_t xReturn = pdPASS;
	
	printf("事件实验\n");
	
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

