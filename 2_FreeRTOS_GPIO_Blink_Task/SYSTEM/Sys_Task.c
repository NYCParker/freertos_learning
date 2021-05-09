#include "Sys_Task.h"
#include "list.h"
#include "Task.h"
#include "port.h"
#include "GPIO.h"
#include <xmc_gpio.h>

//定义任务栈,任务句柄和任务控制块
TaskHandle_t Task1_Handle;
#define TASK1_STACK_SIZE 128    
StackType_t Task1Stack[TASK1_STACK_SIZE];   //512个字节 FreeRTOS推荐的最小的任务栈 为任务堆栈开辟的内存 首位地址在数组第一个元素 栈空时 栈顶在最后一个元素的位置
TCB_t Task1TCB;

TaskHandle_t Task2_Handle;
#define TASK2_STACK_SIZE 128
StackType_t Task2Stack[TASK2_STACK_SIZE];
TCB_t Task2TCB;


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
		delay_Xms(500);
		XMC_GPIO_ToggleOutput(GPIO_LED1);
		taskYIELD();
	}
}

//Task2
void Task2_Entry(void *p_arg)
{	
	for( ;; )
	{
		delay_Xms(500);
		XMC_GPIO_ToggleOutput(GPIO_LED2);
		taskYIELD();
	}
}

void LED_Blink_Task(void)
{
	//创建任务
	Task1_Handle = xTaskCreateStatic((TaskFunction_t)Task1_Entry , (char*)"Task1", (uint32_t)TASK1_STACK_SIZE, (void*) NULL, (StackType_t*)Task1Stack, (TCB_t* )&Task1TCB);
	Task2_Handle = xTaskCreateStatic((TaskFunction_t)Task2_Entry , (char*)"Task2", (uint32_t)TASK2_STACK_SIZE, (void*) NULL, (StackType_t*)Task2Stack, (TCB_t* )&Task2TCB);
	
	//初始化就绪列表
	prvInitialiseTaskLists();
	
	//将任务添加到就绪列表
	vListInsertEnd(&(pxReadyTasksList[1]),&( ((TCB_t*)&Task1TCB)->xStateListItem));
	vListInsertEnd(&(pxReadyTasksList[2]),&( ((TCB_t*)&Task2TCB)->xStateListItem));
	
	//开启任务调度
	vTaskStartScheduler();
}

