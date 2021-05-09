#include "Task.h"
#include "list.h"

//定义就绪列表
List_t pxReadyTasksLists[configMAX_PRIORITIES];  //同一优先级的任务就插入到就绪列表的同一条链表中
TCB_t* volatile pxCurrentTCB = NULL;


static volatile TickType_t xTickCount = ( TickType_t ) configINITIAL_TICK_COUNT;
static volatile UBaseType_t uxCurrentNumberOfTasks = ( UBaseType_t ) 0U; //全局任务计时器

//将任务添加到就序列表
#define prvAddTaskToReadyList( pxTCB )\
		taskRECORD_READY_PRIORITY( ( pxTCB )->uxPriority );\
		vListInsertEnd( &( pxReadyTasksLists[ ( pxTCB )->uxPriority ] ),\
		               &( ( pxTCB )->xStateListItem ) );
					   
//找到优先级最高的就绪任务，赋值PCB，以及非就绪时的处理（不同的操作地现在uxReadyPriorities位的变换上，并处理得到信息）
#if ( configUSE_PORT_OPTIMISED_TASK_SELECTION == 0 )
//普通方法
#else
//优化方法
	#define taskRECORD_READY_PRIORITY( uxPriority )\
	{\
		portRECORD_READY_PRIORITY( uxPriority, uxTopReadyPriority );\
	}
	
    //找到就绪列表中优先级最高的任务的任务控制块给当前任务指针	
    #define taskSELECT_HIGHEST_PRIORITY_TASK()\
	        {\
				UBaseType_t uxTopPriority;\
			    portGET_HIGHEST_PRIORITY( uxTopPriority, uxTopReadyPriority );\
				listGET_OWNER_OF_NEXT_ENTRY(pxCurrentTCB, &(pxReadyTasksLists[uxTopPriority]));\
			}
	#if 0
	//暂未实现		
	#else
		#define taskRESET_READY_PRIORITY( uxPriority )\
		{\
			portRESET_READY_PRIORITY((uxPriority ), (uxTopReadyPriority));\
		}
	#endif
#endif		
		
//静态任务创建函数（需要自己指定任务栈）
#if( configSUPPORT_STATIC_ALLOCATION == 1 )

TaskHandle_t xTaskCreateStatic(	TaskFunction_t pxTaskCode,           //任务入口地址（函数名） 类型定义了一个指向任务函数的指针
					            const char * const pcName,           //指针 任务名称地址
					            const uint32_t ulStackDepth,         //任务栈大小，单位为字
					            void * const pvParameters,           //指向传入任务参数 （传入任务参数的地址，方便理解）
								UBaseType_t uxPriority,
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
		                  uxPriority,
                          &xReturn,          //*****任务句柄 这里用&xReturn 指针的地址 所以这个函数定义的时候是个二级指针
                          pxNewTCB);         //任务栈起始地址
		
	    prvAddNewTaskToReadyList( pxNewTCB ); //将任务添加到就绪列表
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
									UBaseType_t uxPriority,
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
	
	//初始化优先级
	if( uxPriority >= ( UBaseType_t ) configMAX_PRIORITIES )
	{
		uxPriority = ( UBaseType_t ) configMAX_PRIORITIES - ( UBaseType_t ) 1U;
	}
	pxNewTCB->uxPriority = uxPriority;
	
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
		vListInitialise( &(pxReadyTasksLists[uxPriority]) );
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
										 (UBaseType_t) tskIDLE_PRIORITY,	 
										 (StackType_t *)pxIdleTaskStackBuffer, 
										 (TCB_t *)pxIdleTaskTCBBuffer);
										 
    xTickCount = ( TickType_t ) configINITIAL_TICK_COUNT;									 
	//	
	
	if( xPortStartScheduler()!=pdFALSE)
	{
		
	}
}

//手动切换程序
//要改成pxCurrenTCB 在任务切换的时候指向最高优先级的就绪任务的 TCB 
void vTaskSwitchContext( void )
{
	taskSELECT_HIGHEST_PRIORITY_TASK();
}


void vTaskDelay(const TickType_t xTickToDelay)
{
	TCB_t *pxTCB = NULL;
	
	pxTCB = pxCurrentTCB;   //获取当前任务的TCB
	
	pxTCB->xTicksToDelay = xTickToDelay; //设置延时时间
	
	//将任务从就绪列表移除（因为进入延时）
	taskRESET_READY_PRIORITY(pxTCB->uxPriority);  //这里先简单的清除对应的优先级位
	
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
			pxTCB->xTicksToDelay--;   //相当于每进一次SYS中断就扫描所有任务，如果其xTicksToDelay不为0，就减1（减到0切换到该任务就实现了任务延迟）
			
			if(pxTCB->xTicksToDelay == 0)
			{
				 taskRECORD_READY_PRIORITY(pxTCB->uxPriority);
			}
		}
	}
	portYIELD();
}

//指定任务控制块和将任务添加到就绪列表
static void prvAddNewTaskToReadyList( TCB_t* pxNewTCB)
{
	taskENTER_CRITICAL();   //进入临界区
	{
		uxCurrentNumberOfTasks++;
		
		if(pxCurrentTCB == NULL)    //如果pxCurrent为空，则将 pxCurrentTCB 指向新创建的任务
		{
			pxCurrentTCB = pxNewTCB;
			
			if(uxCurrentNumberOfTasks == (UBaseType_t)1)   //说明第一个任务刚刚创建 需要初始化任务相关的列表
			{
				prvInitialiseTaskLists();
			}
		}
		else  //如果pxCurrent不为控，那么创建一个新的任务来说，如果新的任务优先级更高，就跟换当前要执行的任务到新任务的地址
	    {
			if(pxCurrentTCB->uxPriority <= pxNewTCB->uxPriority)
			{
				pxCurrentTCB = pxNewTCB;
			}
		}
		prvAddTaskToReadyList(pxNewTCB);    //将任务添加到就绪列表
	}
	taskEXIT_CRITICAL();
}

