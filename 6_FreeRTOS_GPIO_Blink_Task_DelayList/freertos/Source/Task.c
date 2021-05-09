#include "Task.h"
#include "list.h"

//��������б�
List_t pxReadyTasksLists[configMAX_PRIORITIES];  //ͬһ���ȼ�������Ͳ��뵽�����б��ͬһ��������
TCB_t* volatile pxCurrentTCB = NULL;

static volatile UBaseType_t uxTopReadyPriority = tskIDLE_PRIORITY;  //uxTopReadyPriority���ִ��������������ȼ�		

static volatile TickType_t xTickCount = ( TickType_t ) configINITIAL_TICK_COUNT;
static volatile UBaseType_t uxCurrentNumberOfTasks = ( UBaseType_t ) 0U; //ȫ�������ʱ��

//������ʱ�б�
static List_t xDelayedTaskList1;
static List_t xDelayedTaskList2;
static List_t* volatile pxDelayedTaskList;         //ָ��ǰʹ�õ���ʱ�б�
static List_t* volatile pxOverflowDelayedTaskList; //ָ���������ʱ�б�

//��һ��������ʱ���ھ���ʱ��
static volatile TickType_t xNextTaskUnblockTime = ( TickType_t ) 0U;  //��������ʼ����Ҫ���portMAX_DELAY

//�������
static volatile BaseType_t xNumOfOverflows = ( BaseType_t ) 0;

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
				UBaseType_t uxTopPriority = 0;\
			    portGET_HIGHEST_PRIORITY( uxTopPriority, uxTopReadyPriority );\
				listGET_OWNER_OF_NEXT_ENTRY(pxCurrentTCB, &(pxReadyTasksLists[uxTopPriority]));\
			}
	#if 0
	//��δʵ��		
	#else
	   //���uxPriority���ȼ��������Ƴ�����������ʱ�б���ôͬһ�����ȼ��²�һ����һ�����񣬲���������������ȼ�
		#define taskRESET_READY_PRIORITY( uxPriority )\
		{\
			if(listCURRENT_LIST_LENGTH( &( pxReadyTasksLists[ ( uxPriority ) ] ) ) == ( UBaseType_t ) 0 )\
			{\
				portRESET_READY_PRIORITY((uxPriority ), (uxTopReadyPriority));\
			}\
		}
	#endif
#endif		

//������л���ʱ�б���λxNextTaskUnblockTime��ֵ
#define taskSWITCH_DELAYED_LISTS()\
{\
	List_t *pxTemp;\
	pxTemp = pxDelayedTaskList;\
	pxDelayedTaskList = pxOverflowDelayedTaskList;\
	pxOverflowDelayedTaskList = pxTemp;\
	xNumOfOverflows++;\
	prvResetNextTaskUnblockTime();\
}
		
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
	
	//��ʼ��������ʱ�б�
	vListInitialise( &xDelayedTaskList1 );
	vListInitialise( &xDelayedTaskList2 );
	pxDelayedTaskList = &xDelayedTaskList1;
	pxOverflowDelayedTaskList = &xDelayedTaskList2;
	
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
	
	xNextTaskUnblockTime = portMAX_DELAY;	
										 
    xTickCount = ( TickType_t ) configINITIAL_TICK_COUNT;									 
	
	
	if( xPortStartScheduler()!=pdFALSE)
	{
		
	}
}

//д��������
void listGET_OWNER_OF_NEXT_ENTRY1(List_t * const pxList )    
{
	List_t * const pxConstList = pxList;
	pxConstList->pxIndex = pxConstList->pxIndex->pxNext;
	if( ( void * )pxConstList->pxIndex == (void *)&(pxConstList->xListEnd))
	{
		pxConstList->pxIndex = pxConstList->pxIndex->pxNext; 
	}
	pxCurrentTCB = pxConstList->pxIndex->pvOwner;
	
}

//�ֶ��л�����
//Ҫ�ĳ�pxCurrenTCB �������л���ʱ��ָ��������ȼ��ľ�������� TCB 
void vTaskSwitchContext( void )
{
	UBaseType_t uxTopPriority = 0;
	portGET_HIGHEST_PRIORITY( uxTopPriority, uxTopReadyPriority );
	listGET_OWNER_OF_NEXT_ENTRY1(&(pxReadyTasksLists[uxTopPriority]));
	//taskSELECT_HIGHEST_PRIORITY_TASK
}


void vTaskDelay(const TickType_t xTicksToDelay)
{
	TCB_t *pxTCB = NULL;	
	pxTCB = pxCurrentTCB;   //��ȡ��ǰ�����TCB
	
	prvAddCurrentTaskToDelayedList(xTicksToDelay);
	
	taskYIELD();
}

void xTaskIncrementTick( void )
{
	TCB_t *pxTCB;
	TickType_t xItemValue;
	
	const TickType_t xConstTickCount1 = xTickCount + 1;
    xTickCount = xConstTickCount1;	
	
	if(xConstTickCount1 ==( (TickType_t) 0U ))
	{
		//���������л���ʱ�б�
		taskSWITCH_DELAYED_LISTS();
	}
	
	if(xConstTickCount1 >= xNextTaskUnblockTime)   //�����ʱ����
	{
		for ( ; ; )
		{
			if(listLIST_IS_EMPTY( pxDelayedTaskList ) != pdFALSE)
			{
				xNextTaskUnblockTime = portMAX_DELAY;
				break;
			}
			else
			{
				pxTCB = listGET_OWNER_OF_HEAD_ENTRY( pxDelayedTaskList );
				xItemValue = listGET_LIST_ITEM_VALUE( &(pxTCB->xStateListItem) );
			
				if(xConstTickCount1 < xItemValue)
				{
					xNextTaskUnblockTime = xItemValue;
					break;
				}
				(void)uxListRemove(&(pxTCB->xStateListItem));
				prvAddTaskToReadyList(pxTCB);
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

static void prvAddCurrentTaskToDelayedList( TickType_t xTicksToWait )
{
	TickType_t xTimeToWake;
	
	const TickType_t xConstTickCount = xTickCount;  //��ȡ��ǰʱ��
	
	if(uxListRemove(&(pxCurrentTCB->xStateListItem)) == ( UBaseType_t ) 0)   //ֱ���Ƴ������ȫû�˻�Ҫ��һ�����������Ƴ��б��Ӧ�����ȼ�
	{
		//�ܽ�����˵����ǰ�������ȼ���Ӧ�ľ����б��Ѿ�����
		//taskRESET_READY_PRIORITY(pxCurrentTCB->uxPriority);        //���Ƴ��б�i��Ӧ�����ȼ�
		portRESET_READY_PRIORITY( pxCurrentTCB->uxPriority,uxTopReadyPriority );
	}
	
	xTimeToWake = xConstTickCount + xTicksToWait;
	listSET_LIST_ITEM_VALUE( &( pxCurrentTCB->xStateListItem ),xTimeToWake );
	
	if( xTimeToWake < xConstTickCount)                             //������
	{
		vListInsert(pxOverflowDelayedTaskList, &(pxCurrentTCB->xStateListItem));  //�ѹ��ؽ���һ���б�
	}
	else
	{
		vListInsert(pxDelayedTaskList, &(pxCurrentTCB->xStateListItem));
		//�������ʱ����ڵ�ǰ�����Ҫ�����ӳٵ�������Ҫ������һ�������˳���ʱʱ���
		if( xTimeToWake < xNextTaskUnblockTime )
		{
			xNextTaskUnblockTime = xTimeToWake;
		}
	}
}

static void prvResetNextTaskUnblockTime( void )
{
	TCB_t *pxTCB;
	
	if( listLIST_IS_EMPTY(pxDelayedTaskList) != pdFALSE )  //�����ʱ�б�Ϊ�գ���λxNextTaskUnblockTime
	{
		xNextTaskUnblockTime = portMAX_DELAY;   
	}
	else   //��ǰ�б�Ϊ��,��xNextTaskUnblockTime����Ϊ������ʱ�б���һ���ڵ������ֵ��������ʱ�б��Ѿ����ˣ�����Ҫ����һ��xNextTaskUnblockTime�����ֵ���úܴ�
	{
		(pxTCB) = (TCB_t*)(listGET_OWNER_OF_HEAD_ENTRY(pxDelayedTaskList));
		xNextTaskUnblockTime = listGET_LIST_ITEM_VALUE(&( (pxTCB) ->xStateListItem));
	}
}
