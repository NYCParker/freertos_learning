#ifndef PORT_H
#define PORT_H

#include "list.h"
#include "projdefs.h"

#define portNVIC_SYSPRI2_REG (*(( volatile uint32_t *) 0xe000ed20))
#define portNVIC_PENDSV_PRI (((uint32_t) configKERNEL_INTERRUPT_PRIORITY ) << 16UL)
#define portNVIC_SYSTICK_PRI (((uint32_t) configKERNEL_INTERRUPT_PRIORITY ) << 24UL )

extern StackType_t *pxPortInitialiseStack( StackType_t *pxTopOfStack,     //栈顶指针地址（刚进来的时候栈空，栈顶在高地址位置）
									TaskFunction_t pxCode,         //任务入口地址
									void *pvParameters );          //传递的参数的地址
#endif 

