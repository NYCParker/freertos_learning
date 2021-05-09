#ifndef PORT_H
#define PORT_H

#include "list.h"
#include "projdefs.h"

#define portNVIC_SYSPRI2_REG (*(( volatile uint32_t *) 0xe000ed20))
#define portNVIC_PENDSV_PRI (((uint32_t) configKERNEL_INTERRUPT_PRIORITY ) << 16UL)
#define portNVIC_SYSTICK_PRI (((uint32_t) configKERNEL_INTERRUPT_PRIORITY ) << 24UL )

/* SysTick 控制寄存器 */ 
#define portNVIC_SYSTICK_CTRL_REG (*((volatile uint32_t *) 0xe000e010 ))
/* SysTick 重装载寄存器寄存器 */
#define portNVIC_SYSTICK_LOAD_REG (*((volatile uint32_t *) 0xe000e014 ))
	
///* SysTick 时钟源选择 */
#ifndef configSYSTICK_CLOCK_HZ
	#define configSYSTICK_CLOCK_HZ configCPU_CLOCK_HZ
	#define portNVIC_SYSTICK_CLK_BIT (1UL << 2UL)
#else
	#define portNVIC_SYSTICK_CLK_BIT ( 0 )
#endif
#define portNVIC_SYSTICK_INT_BIT ( 1UL << 1UL )
#define portNVIC_SYSTICK_ENABLE_BIT ( 1UL << 0UL )

extern void vPortEnterCritical(void);
extern void vPortExitCritical( void );
extern StackType_t *pxPortInitialiseStack( StackType_t *pxTopOfStack,     //栈顶指针地址（刚进来的时候栈空，栈顶在高地址位置）
									TaskFunction_t pxCode,         //任务入口地址
									void *pvParameters );          //传递的参数的地址

void vPortSetupTimerInterrupt(void);
#endif 

