#include "Task.h"
#include "list.h"

//��������б�
List_t pxReadyTasksList[configMAX_PRIORITIES];  //ͬһ���ȼ�������Ͳ��뵽�����б��ͬһ��������
TCB_t* volatile pxCurrentTCB = NULL;

static volatile TickType_t xTickCount = ( TickType_t ) configINITIAL_TICK_COUNT;

//��̬���񴴽���������Ҫ�Լ�ָ������ջ��
#if( configSUPPORT_STATIC_ALLOCATION == 1 )

TaskHandle_t xTaskCreateStatic(	TaskFunction_t pxTaskCode,           //������ڵ�ַ���������� ���Ͷ�����һ��ָ����������ָ��
					            const char * const pcName,           //ָ�� �������Ƶ�ַ
					            const uint32_t ulStackDepth,         //����ջ��С����λΪ��
					            void * const pvParameters,           //ָ����������� ��������������ĵ�ַ��������⣩
					            StackType_t * const puxStackBuffer,  //����ջ����ʼ��ַ
					            TCB_t * const pxTaskBuffer )         //������ƿ�ָ��
{
	TCB_t *pxNewTCB;
	TaskHandle_t xReturn;
	
	if((pxTaskBuffer != NULL)&&(puxStackBuffer != NULL))
	{
		pxNewTCB = (TCB_t*) pxTaskBuffer;  //��ַ��pxNewTCB
		pxNewTCB->pxStack = (StackType_t *)puxStackBuffer; /*������ջ����ʼ��ַ����������ƿ�*/
	
	prvInitialiseNewTask( pxTaskCode,        //�������
                          pcName,            //�������ƣ��ַ�����ʽ
                          ulStackDepth,      //����ջ��С����λΪ�� */ 
                          pvParameters,      //�����β� */
                          &xReturn,          //*****������ ������&xReturn ָ��ĵ�ַ ����������������ʱ���Ǹ�����ָ��
                          pxNewTCB);         //����ջ��ʼ��ַ
	}
	else
	{
		xReturn = NULL;
	}
	return xReturn;
}

#endif /* configSUPPORT_STATIC_ALLOCATION */

//
static void prvInitialiseNewTask( 	TaskFunction_t pxTaskCode,              //�������
									const char * const pcName,              //������
									const uint32_t ulStackDepth,            //����ջ��С
									void * const pvParameters,              //���ݸ�����Ĳ���
									TaskHandle_t * const pxCreatedTask,     //*****������TaskHandle_t pxCreatedTask�Ǹ�ָ��ָ���ָ��
									TCB_t *pxNewTCB )                       //������ƿ�ָ��
{
	StackType_t *pxTopOfStack;
	UBaseType_t x;
	
	pxTopOfStack = pxNewTCB->pxStack + (ulStackDepth - (uint32_t)1);    //ջ����ַ��ջ��������������ջ�գ�ջ��ָ����������һ��Ԫ�أ������ڴ����ߵ�ַ��
	pxTopOfStack = (StackType_t *) \
	               ( ( ( uint32_t ) pxTopOfStack ) & ( ~( ( uint32_t ) 0x0007 ) ) ); //������8�ֽڶ��루��Ը��㲻�ã���֤M4��ַ���ʲ������⣩
	
	for(x=(UBaseType_t)0;  x<(UBaseType_t) configMAX_TASK_NAME_LEN; x++) /*���������ִ��ݸ�������ƿ�*/
	{                                                                    //pcName��Ϊͬ��ָ�룬�����ֱ������������
		pxNewTCB->pctaskName[x] = pcName[x];
		if(pcName[x] == 0x00)
		{
			break;
		}
	}
	pxNewTCB->pctaskName[configMAX_TASK_NAME_LEN-1] = '\0';
	
	vListInitialiseItem(&(pxNewTCB->xStateListItem));   /*��ʼ��������ƿ��е�����ڵ�*/
	listSET_LIST_ITEM_OWNER(&(pxNewTCB->xStateListItem),pxNewTCB); /*��ʼ������ڵ��pvOwner��ַΪpxNewTCB�ĵ�ַ����ָ��������ƿ飩*/
	
	pxNewTCB->pxTopOfStack = pxPortInitialiseStack(pxTopOfStack, pxTaskCode, pvParameters);  //��ʼ������ջ
	
	if((void*)pxCreatedTask != NULL)
	{
		*pxCreatedTask = (TaskFunction_t)pxNewTCB;         //��ϵ���������ʹ�ã�pxCreatedTask����ָ��xreturn�ĵ�ַ����xreturn = (TaskFunction_t)pxNewTCB��
		                                                   //���񴴽��������ܷ���һ��ָ��������ƿ��ָ��xReturn
	}	
}

//��ʼ�������б�
void prvInitialiseTaskLists(void)
{
	UBaseType_t uxPriority;
	
	for(uxPriority = ( UBaseType_t ) 0U; uxPriority < ( UBaseType_t ) configMAX_PRIORITIES; uxPriority++)
	{
		vListInitialise( &(pxReadyTasksList[uxPriority]) );
	}
}

//�ֶ�������
void vTaskStartScheduler(void)
{
	//--�������񴴽��������ص������б�0��
	TCB_t* pxIdleTaskTCBBuffer = NULL;
    StackType_t *pxIdleTaskStackBuffer = NULL;
	uint32_t ulIdleTaskStackSize;
	
	vApplicationGetIdleTaskMemory(&pxIdleTaskTCBBuffer,&pxIdleTaskStackBuffer,&ulIdleTaskStackSize);
	
	xIdleTaskHandle = xTaskCreateStatic( (TaskFunction_t)prvIdleTask , 
										 (char *)"IDLE", 
										 (uint32_t)ulIdleTaskStackSize, 
										 (void *)NULL, 
										 (StackType_t *)pxIdleTaskStackBuffer, 
										 (TCB_t *)pxIdleTaskTCBBuffer);

	vListInsertEnd(&pxReadyTasksList[0],&((TCB_t*)pxIdleTaskTCBBuffer)->xStateListItem);	
										 
	//
										 
    xTickCount = ( TickType_t ) configINITIAL_TICK_COUNT;									 
	//	
										 
	pxCurrentTCB = &Task1TCB;      //�ֶ�ָ����һ�����е�����
	
	if( xPortStartScheduler()!=pdFALSE)
	{
		
	}
}


#if 0
void vTaskSwitchContext( void )
{
	if ( pxCurrentTCB == &Task1TCB )   //�������������л�
	{
		pxCurrentTCB = &Task2TCB;
	}
	else
	{
		pxCurrentTCB = &Task1TCB;
	}
}
#else //�ú�����������Ѱ��������ȼ��ľ�������
void vTaskSwitchContext( void )
{
	if(pxCurrentTCB == &IdleTaskTCB)
	{
		if(Task1TCB.xTicksToDelay == 0)
		{
			pxCurrentTCB = &Task1TCB;
		}
		else if(Task2TCB.xTicksToDelay == 0)
		{
			pxCurrentTCB = &Task2TCB;
		}
		else
		{
			return;               //����ִ�п�������
		}
	}
	else //�����ǰ�����ǿ�������
	{
		if(pxCurrentTCB == &Task1TCB)
		{
			if(Task2TCB.xTicksToDelay == 0)
			{
				pxCurrentTCB = &Task2TCB;
			}
			else if(pxCurrentTCB->xTicksToDelay != 0)
			{
				pxCurrentTCB = &IdleTaskTCB;
			}
			else
			{
				return;        //���������ʱ������һ��ʱ�������������л�����������1
			}
		}
		
		else if (pxCurrentTCB == &Task2TCB)
		{
			if (Task1TCB.xTicksToDelay == 0)
			{
				pxCurrentTCB =&Task1TCB;
			}
			else if (pxCurrentTCB->xTicksToDelay != 0)
			{
				pxCurrentTCB = &IdleTaskTCB;
			}
			else
			{
				return; 
			}
		}
	}		
}


void vTaskDelay(const TickType_t xTickToDelay)
{
	TCB_t *pxTCB = NULL;
	
	pxTCB = pxCurrentTCB;   //��ȡ��ǰ�����TCB
	
	pxTCB->xTicksToDelay = xTickToDelay; //������ʱʱ��
	
	taskYIELD();
}

void xTaskIncrementTick( void )
{
	TCB_t *pxTCB = NULL;
    BaseType_t i = 0;
	const TickType_t xConstTickCount = xTickCount + 1;
    xTickCount = xConstTickCount;
	
	for(i=0; i<3;i++)
	{
		pxTCB = (TCB_t*)listGET_OWNER_OF_HEAD_ENTRY(&pxReadyTasksList[i]);
		if(pxTCB->xTicksToDelay > 0)
		{
			pxTCB->xTicksToDelay--;   //�൱��ÿ��һ��SYS�жϾ�ɨ���������������xTicksToDelay��Ϊ0���ͼ�1������0�л����������ʵ���������ӳ٣�
		}
	}
	portYIELD();
}



#endif
