
#ifndef PORTMACRO_H
#define PORTMACRO_H

#include "stdint.h"   //<stdint.h>中定义了几种扩展的整数类型和宏
#include "stddef.h"   //stddef.h定义了一些标准宏以及类型.   NULL 空指针常量
#include "FreeRTOSConfig.h"

/* Type definitions. */
    #define portCHAR          char
    #define portFLOAT         float
    #define portDOUBLE        double
    #define portLONG          long
    #define portSHORT         short
    #define portSTACK_TYPE    uint32_t
    #define portBASE_TYPE     long

    typedef portSTACK_TYPE   StackType_t;      //uint32
    typedef long             BaseType_t;       //int32
    typedef unsigned long    UBaseType_t;      //uint32

	#if( configUSE_16_BIT_TICKS == 1 ) 
	typedef uint16_t TickType_t;
	#define portMAX_DELAY ( TickType_t ) 0xffff
	#else
	typedef uint32_t TickType_t;
	#define portMAX_DELAY ( TickType_t ) 0xffffffffUL     //宏configUSE_16_BIT_TICKS默认为0 TickType_t表示32位 uint32
	#endif
	
#define portNVIC_INT_CTRL_REG (*(( volatile uint32_t *) 0xe000ed04)) 
#define portNVIC_PENDSVSET_BIT ( 1UL << 28UL )	
#define portSY_FULL_READ_WRITE ( 15 )

#define portFORCE_INLINE    __forceinline

#define portYIELD() \
{ \
/* 触发 PendSV，产生上下文切换 */ \
portNVIC_INT_CTRL_REG = portNVIC_PENDSVSET_BIT; \
__dsb( portSY_FULL_READ_WRITE ); \
__isb( portSY_FULL_READ_WRITE ); \
}
//portYIELD实际就是将 PendSV 的悬起位置 1，当没有其它中断运行的时候响应 PendSV 中断(优先级最低），
//去执行我们写好的 PendSV中断服务函数，在里面实现任务切换
//任务切换的本质是通过触发 PendSV 中断，在中断中执行任务切换

#define portDISABLE_INTERRUPTS() vPortRaiseBASEPRI()

//不带返回值，因为没有保存当前basepri寄存器的状态
static portFORCE_INLINE void vPortRaiseBASEPRI( void )
{
	uint32_t ulNewBASEPRI = configMAX_SYSCALL_INTERRUPT_PRIORITY;
	__asm
	{
		msr basepri, ulNewBASEPRI
		dsb
		isb
	}
}

//带返回值的关中断函数
#define portSET_INTERRUPT_MASK_FROM_ISR() ulPortRaiseBASEPRI()
static portFORCE_INLINE uint32_t ulPortRaiseBASEPRI(void)
{
	uint32_t ulReturn,ulNewBASEPRI = configMAX_SYSCALL_INTERRUPT_PRIORITY;
	__asm
	{
		mrs ulReturn, basepri;
		msr basepri, ulNewBASEPRI
		dsb
		isb		
	}
	return ulReturn;
}	

//带中断保护的开中断函数
#define portENABLE_INTERRUPTS() vPortSetBASEPRI( 0 )
#define portCLEAR_INTERRUPT_MASK_FROM_ISR(x) vPortSetBASEPRI(x)

static portFORCE_INLINE void vPortSetBASEPRI( uint32_t ulBASEPRI )  //搭配portSET_INTERRUPT_MASK_FROM_ISR如何使用？
{
	__asm
	{
		msr basepri, ulBASEPRI
	}
}

//进入和退出临界段的宏最终也是通过开/关中断实现的
#define portENTER_CRITICAL() vPortEnterCritical()  //进入临界段
#define taskENTER_CRITICAL_FROM_ISR() portSET_INTERRUPT_MASK_FROM_ISR()
#define portEXIT_CRITICAL() vPortExitCritical()
#define taskEXIT_CRITICAL_FROM_ISR( x ) portCLEAR_INTERRUPT_MASK_FROM_ISR( x )


#endif /* PORTMACRO_H */
