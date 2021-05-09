#include "Task.h"
#include "list.h"

//��������б�
List_t pxReadyTasksLists[configMAX_PRIORITIES];  //ͬһ���ȼ�������Ͳ��뵽�����б��ͬһ��������
TCB_t* volatile pxCurrentTCB = NULL;


static volatile TickType_t xTickCount = ( TickType_t ) configINITIAL_TICK_COUNT;
static volatile UBaseType_t uxCurrentNumberOfTasks = ( UBaseType_t ) 0U; //ȫ�������ʱ��

//��������ӵ������б�
#define prvAddTaskToReadyList( pxTCB )\
		taskRECORD_READY_PRIORITY( ( pxTCB )->uxPriority );\
		vListInsertEnd( &( pxReadyTasksLists[ ( pxTCB )->uxPriority ] ),\
		               &( ( pxTCB )->xStateListItem ) );
					   
//�ҵ����ȼ���ߵľ������񣬸�ֵPCB���Լ��Ǿ���ʱ�Ĵ�����ͬ�Ĳ���������uxReadyPrioritiesλ�ı任�ϣ�������õ���Ϣ��
#if ( configUSE_PORT_OPTIMISED_TASK_SELECTION == 0 )
//��ͨ����
#else
//�Ż�����
	#define taskRECORD_READY_PRIORITY( uxPriority )\
	{\
		portRECORD_READY_PRIORITY( uxPriority, uxTopReadyPriority );\
	}
	
    //�ҵ������б������ȼ���ߵ������������ƿ����ǰ����ָ��	
    #define taskSELECT_HIGHEST_PRIORITY_TASK()\
	        {\
				UBaseType_t uxTopPriority;\
			    portGET_HIGHEST_PRIORITY( uxTopPriority, uxTopReadyPriority );\
				listGET_OWNER_OF_NEXT_ENTRY(pxCurrentTCB, &(pxReadyTasksLists[uxTopPriority]));\
			}
	#if 0
	//��δʵ��		
	#else
		#define taskRESET_READY_PRIORITY( uxPriority )\
		{\
			portRESET_READY_PRIORITY((uxPriority ), (uxTopReadyPriority));\
		}
	#endif
#endif		
		
//��̬���񴴽���������Ҫ�Լ�ָ������ջ��
#if( configSUPPORT_STATIC_ALLOCATION == 1 )

TaskHandle_t xTaskCreateStatic(	TaskFunction_t pxTaskCode,           //������ڵ�ַ���������� ���Ͷ�����һ��ָ����������ָ��
					            const char * const pcName,           //ָ�� �������Ƶ�ַ
					            const uint32_t ulStackDepth,         //����ջ��С����λΪ��
					            void * const pvParameters,           //ָ����������� ��������������ĵ�ַ��������⣩
								UBaseType_t uxPriority,
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
		                  uxPriority,
                          &xReturn,          //*****������ ������&xReturn ָ��ĵ�ַ ����������������ʱ���Ǹ�����ָ��
                          pxNewTCB);         //����ջ��ʼ��ַ
		
	    prvAddNewTaskToReadyList( pxNewTCB ); //��������ӵ������б�
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
									UBaseType_t uxPriority,
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
	
	//��ʼ�����ȼ�
	if( uxPriority >= ( UBaseType_t ) configMAX_PRIORITIES )
	{
		uxPriority = ( UBaseType_t ) configMAX_PRIORITIES - ( UBaseType_t ) 1U;
	}
	pxNewTCB->uxPriority = uxPriority;
	
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
		vListInitialise( &(pxReadyTasksLists[uxPriority]) );
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
										 (UBaseType_t) tskIDLE_PRIORITY,	 
										 (StackType_t *)pxIdleTaskStackBuffer, 
										 (TCB_t *)pxIdleTaskTCBBuffer);
										 
    xTickCount = ( TickType_t ) configINITIAL_TICK_COUNT;									 
	//	
	
	if( xPortStartScheduler()!=pdFALSE)
	{
		
	}
}

//�ֶ��л�����
//Ҫ�ĳ�pxCurrenTCB �������л���ʱ��ָ��������ȼ��ľ�������� TCB 
void vTaskSwitchContext( void )
{
	taskSELECT_HIGHEST_PRIORITY_TASK();
}


void vTaskDelay(const TickType_t xTickToDelay)
{
	TCB_t *pxTCB = NULL;
	
	pxTCB = pxCurrentTCB;   //��ȡ��ǰ�����TCB
	
	pxTCB->xTicksToDelay = xTickToDelay; //������ʱʱ��
	
	//������Ӿ����б��Ƴ�����Ϊ������ʱ��
	taskRESET_READY_PRIORITY(pxTCB->uxPriority);  //�����ȼ򵥵������Ӧ�����ȼ�λ
	
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
		pxTCB = (TCB_t*)listGET_OWNER_OF_HEAD_ENTRY(&pxReadyTasksLists[i]);
		if(pxTCB->xTicksToDelay > 0)
		{
			pxTCB->xTicksToDelay--;   //�൱��ÿ��һ��SYS�жϾ�ɨ���������������xTicksToDelay��Ϊ0���ͼ�1������0�л����������ʵ���������ӳ٣�
			
			if(pxTCB->xTicksToDelay == 0)
			{
				 taskRECORD_READY_PRIORITY(pxTCB->uxPriority);
			}
		}
	}
	portYIELD();
}

//ָ��������ƿ�ͽ�������ӵ������б�
static void prvAddNewTaskToReadyList( TCB_t* pxNewTCB)
{
	taskENTER_CRITICAL();   //�����ٽ���
	{
		uxCurrentNumberOfTasks++;
		
		if(pxCurrentTCB == NULL)    //���pxCurrentΪ�գ��� pxCurrentTCB ָ���´���������
		{
			pxCurrentTCB = pxNewTCB;
			
			if(uxCurrentNumberOfTasks == (UBaseType_t)1)   //˵����һ������ոմ��� ��Ҫ��ʼ��������ص��б�
			{
				prvInitialiseTaskLists();
			}
		}
		else  //���pxCurrent��Ϊ�أ���ô����һ���µ�������˵������µ��������ȼ����ߣ��͸�����ǰҪִ�е�����������ĵ�ַ
	    {
			if(pxCurrentTCB->uxPriority <= pxNewTCB->uxPriority)
			{
				pxCurrentTCB = pxNewTCB;
			}
		}
		prvAddTaskToReadyList(pxNewTCB);    //��������ӵ������б�
	}
	taskEXIT_CRITICAL();
}

