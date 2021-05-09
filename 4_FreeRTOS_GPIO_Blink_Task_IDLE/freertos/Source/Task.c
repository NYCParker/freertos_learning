#include "Task.h"
#include "list.h"

//定义就绪列表
List_t pxReadyTasksList[configMAX_PRIORITIES];  //同一优先级的任务就插入到就绪列表的同一条链表中
TCB_t* volatile pxCurrentTCB = NULL;

static volatile TickType_t xTickCount = ( TickType_t ) configINITIAL_TICK_COUNT;

//静态任务创建函数（需要自己指定任务栈）
#if( configSUPPORT_STATIC_ALLOCATION == 1 )

TaskHandle_t xTaskCreateStatic(	TaskFunction_t pxTaskCode,           //任务入口地址（函数名） 类型定义了一个指向任务函数的指针
					            const char * const pcName,           //指针 任务名称地址
					            const uint32_t ulStackDepth,         //任务栈大小，单位为字
					            void * const pvParameters,           //指向传入任务参数 （传入任务参数的地址，方便理解）
					            StackType_t * const puxStackBuffer,  //任务栈的起始地址
					            TCB_t * const pxTaskBuffer )         //任务控制块指针
{
	TCB_t *pxNewTCB;
	TaskHandle_t xReturn;
	
	if((pxTaskBuffer != NULL)&&(puxStackBuffer != NULL))
	{
		pxNewTCB = (TCB_t*) pxTaskBuffer;  //地址给pxNewTCB
		pxNewTCB->pxStack = (StackType_t *)puxStackBuffer; /*将任务栈的起始地址储存任务控制块*/
	
	prvInitialiseNewTask( pxTaskCode,        //任务入口
                          pcName,            //任务名称，字符串形式
                          ulStackDepth,      //任务栈大小，单位为字 */ 
                          pvParameters,      //任务形参 */
                          &xReturn,          //*****任务句柄 这里用&xReturn 指针的地址 所以这个函数定义的时候是个二级指针
                          pxNewTCB);         //任务栈起始地址
	}
	else
	{
		xReturn = NULL;
	}
	return xReturn;
}

#endif /* configSUPPORT_STATIC_ALLOCATION */

//
static void prvInitialiseNewTask( 	TaskFunction_t pxTaskCode,              //任务入口
									const char * const pcName,              //函数名
									const uint32_t ulStackDepth,            //任务栈大小
									void * const pvParameters,              //传递给任务的参数
									TaskHandle_t * const pxCreatedTask,     //*****任务句柄TaskHandle_t pxCreatedTask是个指向指针的指针
									TCB_t *pxNewTCB )                       //任务控制块指针
{
	StackType_t *pxTopOfStack;
	UBaseType_t x;
	
	pxTopOfStack = pxNewTCB->pxStack + (ulStackDepth - (uint32_t)1);    //栈顶地址（栈向下增长，现在栈空，栈顶指向数组的最后一个元素（开辟内存的最高地址）
	pxTopOfStack = (StackType_t *) \
	               ( ( ( uint32_t ) pxTopOfStack ) & ( ~( ( uint32_t ) 0x0007 ) ) ); //向下做8字节对齐（宁愿舍点不用，保证M4地址访问不出问题）
	
	for(x=(UBaseType_t)0;  x<(UBaseType_t) configMAX_TASK_NAME_LEN; x++) /*将任务名字传递给任务控制块*/
	{                                                                    //pcName作为同名指针，这里又变成数组来用了
		pxNewTCB->pctaskName[x] = pcName[x];
		if(pcName[x] == 0x00)
		{
			break;
		}
	}
	pxNewTCB->pctaskName[configMAX_TASK_NAME_LEN-1] = '\0';
	
	vListInitialiseItem(&(pxNewTCB->xStateListItem));   /*初始化任务控制块中的链表节点*/
	listSET_LIST_ITEM_OWNER(&(pxNewTCB->xStateListItem),pxNewTCB); /*初始化链表节点的pvOwner地址为pxNewTCB的地址（即指向任务控制块）*/
	
	pxNewTCB->pxTopOfStack = pxPortInitialiseStack(pxTopOfStack, pxTaskCode, pvParameters);  //初始化任务栈
	
	if((void*)pxCreatedTask != NULL)
	{
		*pxCreatedTask = (TaskFunction_t)pxNewTCB;         //联系函数定义和使用，pxCreatedTask保存指针xreturn的地址，即xreturn = (TaskFunction_t)pxNewTCB；
		                                                   //任务创建函数就能返回一个指向任务控制快的指针xReturn
	}	
}

//初始化就绪列表
void prvInitialiseTaskLists(void)
{
	UBaseType_t uxPriority;
	
	for(uxPriority = ( UBaseType_t ) 0U; uxPriority < ( UBaseType_t ) configMAX_PRIORITIES; uxPriority++)
	{
		vListInitialise( &(pxReadyTasksList[uxPriority]) );
	}
}

//手动调度器
void vTaskStartScheduler(void)
{
	//--空闲任务创建，并挂载到就绪列表【0】
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
										 
	pxCurrentTCB = &Task1TCB;      //手动指定第一个运行的任务
	
	if( xPortStartScheduler()!=pdFALSE)
	{
		
	}
}


#if 0
void vTaskSwitchContext( void )
{
	if ( pxCurrentTCB == &Task1TCB )   //两个任务轮流切换
	{
		pxCurrentTCB = &Task2TCB;
	}
	else
	{
		pxCurrentTCB = &Task1TCB;
	}
}
#else //该函数正常是是寻找最高优先级的就绪任务
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
			return;               //继续执行空闲任务
		}
	}
	else //如果当前任务不是空闲任务
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
				return;        //任务二在延时，任务一延时结束，不进行切换，继续任务1
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
	
	pxTCB = pxCurrentTCB;   //获取当前任务的TCB
	
	pxTCB->xTicksToDelay = xTickToDelay; //设置延时时间
	
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
			pxTCB->xTicksToDelay--;   //相当于每进一次SYS中断就扫描所有任务，如果其xTicksToDelay不为0，就减1（减到0切换到该任务就实现了任务延迟）
		}
	}
	portYIELD();
}



#endif
