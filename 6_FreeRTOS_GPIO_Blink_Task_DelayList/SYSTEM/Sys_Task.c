#include "Sys_Task.h"
#include "list.h"
#include "Task.h"
#include "port.h"
#include "GPIO.h"
#include <xmc_gpio.h>

//定义任务栈,任务句柄和任务控制块
TCB_t Task1TCB;
TCB_t Task2TCB;

TaskHandle_t Task1_Handle;
#define TASK1_STACK_SIZE 200  
StackType_t Task1Stack[TASK1_STACK_SIZE];   //512个字节 FreeRTOS推荐的最小的任务栈 为任务堆栈开辟的内存 首位地址在数组第一个元素 栈空时 栈顶在最后一个元素的位置

TaskHandle_t Task2_Handle;
#define TASK2_STACK_SIZE 200
StackType_t Task2Stack[TASK2_STACK_SIZE];




//空闲任务
TaskHandle_t xIdleTaskHandle;
#define configMINIMAL_STACK_SIZE ( ( unsigned short ) 200 )
StackType_t IdleTaskStack[configMINIMAL_STACK_SIZE];
TCB_t IdleTaskTCB;  

//
void vApplicationGetIdleTaskMemory( TCB_t **ppxIdleTaskTCBBuffer,          //传入一个指针的地址
									StackType_t **ppxIdleTaskStackBuffer,  //传入一个指针的地址
									uint32_t *pulIdleTaskStackSize )       //传入一个地址
{
	*ppxIdleTaskTCBBuffer = &IdleTaskTCB;  //让指针指向空闲任务控制块
	*ppxIdleTaskStackBuffer = IdleTaskStack; //让指针指向起始地址    //这里为什么要搞这么复杂？
	*pulIdleTaskStackSize=configMINIMAL_STACK_SIZE; //让地址的值为configMINIMAL_STACK_SIZE
}


//software delay
void delay_Xms(uint16_t ticx)        //误差 0us
{
	uint16_t tici;
    unsigned char tica,ticb,ticc;
	
	for(tici=0; tici<ticx; tici++)
    {
	   for(ticc=1;ticc>0;ticc--)
         for(ticb=98;ticb>0;ticb--)
           for(tica=230;tica>0;tica--);
	}
}

//Task1
void Task1_Entry(void *p_arg)
{	
	for( ;; )
	{
		XMC_GPIO_ToggleOutput(GPIO_LED1);
	    vTaskDelay( 500 );
	}
}

//Task2
void Task2_Entry(void *p_arg)
{	
	for( ;; )
	{
		XMC_GPIO_ToggleOutput(GPIO_LED2);
		vTaskDelay( 200 );
	}
}

//空闲任务
void prvIdleTask(void *p_arg)
{
	
}

void LED_Blink_Task(void)
{
	//创建任务
	Task1_Handle = xTaskCreateStatic((TaskFunction_t)Task1_Entry , (char*)"Task1", (uint32_t)TASK1_STACK_SIZE, (void*) NULL, (UBaseType_t) 1, (StackType_t*)Task1Stack, (TCB_t* )&Task1TCB);
	Task2_Handle = xTaskCreateStatic((TaskFunction_t)Task2_Entry , (char*)"Task2", (uint32_t)TASK2_STACK_SIZE, (void*) NULL, (UBaseType_t) 2, (StackType_t*)Task2Stack, (TCB_t* )&Task2TCB);
	
	//开启任务调度
	vTaskStartScheduler();
}

