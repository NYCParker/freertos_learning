
/*************
Author:Yunchuan Ni   FreeRTOS Modification based on Amazon FreeRTOS*****
Description: �����ź���
*************/

/*
main����
*/

/****headfile include****/
#include "main.h"

/*Function declaration*/
static void prvSetupHardware( void );        
extern void GPIO_Init(void);
extern void UART_Init(void);
extern void AppTaskCreate(void);
int fputc(int ch,FILE *f);

/*-----------------------------------------------------------*/

int main( void )
{
	prvSetupHardware();
	AppTaskCreate();
	return 0;
}
/*-----------------------------------------------------------*/

/* Prepare the hardware to run this demo. */
static void prvSetupHardware( void )
{
	NVIC_SetPriorityGrouping( 0 );    //�ж����ȼ�����0 ��ռ���ȼ������λ��1���������ȼ���0��
	                                  //�����λ�ɳ��̾�����XMC4400 0~63�ɱ�����ȼ� bit5��1 ��ռ bit 0 �����ȼ� 63��ͣ�
	GPIO_Init();
	UART_Init();
	
	IRQ_Enable();
}

void vApplicationMallocFailedHook( void )    //�ڴ�����ʧ�ܻ���ô˹��Ӻ���
{
	/* vApplicationMallocFailedHook() will only be called if
	configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
	function that will get called if a call to pvPortMalloc() fails.
	pvPortMalloc() is called internally by the kernel whenever a task, queue,
	timer or semaphore is created.  It is also called by various parts of the
	demo application.  If heap_1.c or heap_2.c are used, then the size of the
	heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
	FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
	to query the size of free heap space that remains (although it does not
	provide information on how the remaining heap might be fragmented). */
	taskDISABLE_INTERRUPTS();
	for( ;; );
}

void vApplicationIdleHook( void )           //���������Ӻ���
{
	/* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
	to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
	task.  It is essential that code added to this hook function never attempts
	to block in any way (for example, call xQueueReceive() with a block time
	specified, or call vTaskDelay()).  If the application makes use of the
	vTaskDelete() API function (as this demo application does) then it is also
	important that vApplicationIdleHook() is permitted to return to its calling
	function, because it is the responsibility of the idle task to clean up
	memory allocated by the kernel to any task that has since been deleted. */
}

void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName ) //��ջ�������ô˹��Ӻ���
{
	( void ) pcTaskName;
	( void ) pxTask;
	taskDISABLE_INTERRUPTS();
	for( ;; );
}

void vApplicationTickHook( void )             //�δ�ʱ�����Ӻ���
{
	/* This function will be called by each tick interrupt if 
	configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h.  User code can be
	added here, but the tick hook is called from an interrupt context, so
	code must not attempt to block, and only the interrupt safe FreeRTOS API
	functions can be used (those that end in FromISR()). */
}

int fputc(int ch,FILE *f)
{
	UART_SendData(ch);
	return ch;
}











