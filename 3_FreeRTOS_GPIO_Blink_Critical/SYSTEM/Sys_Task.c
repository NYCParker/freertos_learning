#include "Sys_Task.h"
#include "list.h"
#include "Task.h"
#include "port.h"
#include "GPIO.h"
#include <xmc_gpio.h>

//��������ջ,��������������ƿ�
TaskHandle_t Task1_Handle;
#define TASK1_STACK_SIZE 128    
StackType_t Task1Stack[TASK1_STACK_SIZE];   //512���ֽ� FreeRTOS�Ƽ�����С������ջ Ϊ�����ջ���ٵ��ڴ� ��λ��ַ�������һ��Ԫ�� ջ��ʱ ջ�������һ��Ԫ�ص�λ��
TCB_t Task1TCB;

TaskHandle_t Task2_Handle;
#define TASK2_STACK_SIZE 128
StackType_t Task2Stack[TASK2_STACK_SIZE];
TCB_t Task2TCB;


//software delay
void delay_Xms(uint16_t ticx)        //��� 0us
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
	//��������
	Task1_Handle = xTaskCreateStatic((TaskFunction_t)Task1_Entry , (char*)"Task1", (uint32_t)TASK1_STACK_SIZE, (void*) NULL, (StackType_t*)Task1Stack, (TCB_t* )&Task1TCB);
	Task2_Handle = xTaskCreateStatic((TaskFunction_t)Task2_Entry , (char*)"Task2", (uint32_t)TASK2_STACK_SIZE, (void*) NULL, (StackType_t*)Task2Stack, (TCB_t* )&Task2TCB);
	
	//��ʼ�������б�
	prvInitialiseTaskLists();
	
	//��������ӵ������б�
	vListInsertEnd(&(pxReadyTasksList[1]),&( ((TCB_t*)&Task1TCB)->xStateListItem));
	vListInsertEnd(&(pxReadyTasksList[2]),&( ((TCB_t*)&Task2TCB)->xStateListItem));
	
	//�����������
	vTaskStartScheduler();
}

