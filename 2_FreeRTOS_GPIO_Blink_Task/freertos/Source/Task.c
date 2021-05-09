#include "Task.h"

//��������б�
List_t pxReadyTasksList[configMAX_PRIORITIES];  //ͬһ���ȼ�������Ͳ��뵽�����б��ͬһ��������
TCB_t* volatile pxCurrentTCB = NULL;

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
	pxCurrentTCB = &Task1TCB;      //�ֶ�ָ����һ�����е�����
	if( xPortStartScheduler()!=pdFALSE)
	{
		
	}
}

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
