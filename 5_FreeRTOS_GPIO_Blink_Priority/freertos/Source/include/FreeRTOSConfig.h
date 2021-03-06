/*在 FreeRTOS 中，定义变量的时候往往会把变量的类型当作前缀加在变量上， 这样的
好处是让用户一看到这个变量就知道该变量的类型。 
比如 char 型变量的前缀是 c， 
short 型变量的前缀是 s， 
long 型变量的前缀是 l， 
portBASE_TYPE 类型变量的前缀是 x。 
还有其他的数据类型，比如数据结构，任务句柄，队列句柄等定义的变量名的前缀也是 x。
如果一个变量是无符号型的那么会有一个前缀 u， 
如果是一个指针变量则会有一个前缀 p
因此，当我们定义一个无符号的 char 型变量的时候会加一个 uc 前缀，
当定义一个char 型的指针变量的时候会有一个 pc 前缀*/

#define xPortPendSVHandler PendSV_Handler
#define xPortSysTickHandler SysTick_Handler
#define vPortSVCHandler SVC_Handler

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#define configUSE_16_BIT_TICKS 0
#define configMAX_TASK_NAME_LEN 16
#define configSUPPORT_STATIC_ALLOCATION 1
#define configMAX_PRIORITIES 5                            //这个5和下面的5要区分 这个是任务的优先级 越小越低

#define configPRIO_BITS       		6 
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY			0x3f //中断最低优先级 63 （中断的优先级，越大越低）
#define configKERNEL_INTERRUPT_PRIORITY 		( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )

#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY	5    //系统可管理的最高优先级（优先级数小于5的中断，不归FreeRTOS管理）
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
/*中断优先级在 5 到 63 的这些中断是受到系统管理，可以在系统操作里被屏蔽的*/

#define configASSERT( x ) if( ( x ) == 0 ) { taskDISABLE_INTERRUPTS(); for( ;; ); }	

//#define configCPU_CLOCK_HZ	( SystemCoreClock )        //CPU频率
#define configCPU_CLOCK_HZ	(( unsigned long ) 120000000)        //CPU频率
#define configTICK_RATE_HZ  (( TickType_t ) 1000)           //每秒中断多少次 

#endif /* FREERTOS_CONFIG_H */
