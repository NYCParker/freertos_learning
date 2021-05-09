#include <XMC4400.h>
#include "Sys_Task.h"
#include "stdio.h"

//simple demo
#define	 LED1_TASK_PRIORITY		( tskIDLE_PRIORITY + 1 )

void Blink_Task( void );

static TaskHandle_t LED1_Task_Handle = NULL;    
static void LED1_Task(void* pvParameters);       

void Blink_Task( void )
{
	BaseType_t xReturn = pdPASS;

	xReturn = xTaskCreate((TaskFunction_t )LED1_Task, 
			(const char* )"LED1_Task",      
			(uint16_t )configMINIMAL_STACK_SIZE,                 
			(void* )NULL,                  
			(UBaseType_t )LED1_TASK_PRIORITY,                
			(TaskHandle_t* )&LED1_Task_Handle);
		
    vTaskStartScheduler();
				
	while(1);
}

static void LED1_Task(void* parameter)
{
	while (1) 
		{
			printf("DEMO Blink\n");
			XMC_GPIO_ToggleOutput(GPIO_LED1);
			vTaskDelay(1000); 
		}		
}
