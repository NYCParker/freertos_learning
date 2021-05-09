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

//��������
TaskHandle_t xIdleTaskHandle;
#define configMINIMAL_STACK_SIZE ( ( unsigned short ) 128 )
StackType_t IdleTaskStack[configMINIMAL_STACK_SIZE];
TCB_t IdleTaskTCB;  

//
void vApplicationGetIdleTaskMemory( TCB_t **ppxIdleTaskTCBBuffer,          //����һ��ָ��ĵ�ַ
									StackType_t **ppxIdleTaskStackBuffer,  //����һ��ָ��ĵ�ַ
									uint32_t *pulIdleTaskStackSize )       //����һ����ַ
{
	*ppxIdleTaskTCBBuffer = &IdleTaskTCB;  //��ָ��ָ�����������ƿ�
	*ppxIdleTaskStackBuffer = IdleTaskStack; //��ָ��ָ����ʼ��ַ    //����ΪʲôҪ����ô���ӣ�
	*pulIdleTaskStackSize=configMINIMAL_STACK_SIZE; //�õ�ַ��ֵΪconfigMINIMAL_STACK_SIZE
}


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
		vTaskDelay( 500 );
	}
}

//��������
void prvIdleTask(void *p_arg)
{
	
}

void LED_Blink_Task(void)
{
	//��������
	Task1_Handle = xTaskCreateStatic((TaskFunction_t)Task1_Entry , (char*)"Task1", (uint32_t)TASK1_STACK_SIZE, (void*) NULL, (UBaseType_t) 1, (StackType_t*)Task1Stack, (TCB_t* )&Task1TCB);
	Task2_Handle = xTaskCreateStatic((TaskFunction_t)Task2_Entry , (char*)"Task2", (uint32_t)TASK2_STACK_SIZE, (void*) NULL, (UBaseType_t) 2, (StackType_t*)Task2Stack, (TCB_t* )&Task2TCB);
	
	//�����������
	vTaskStartScheduler();
}
