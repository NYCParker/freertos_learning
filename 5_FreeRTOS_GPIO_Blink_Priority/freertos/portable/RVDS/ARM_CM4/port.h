#ifndef PORT_H
#define PORT_H

#include "list.h"
#include "projdefs.h"

#define portNVIC_SYSPRI2_REG (*(( volatile uint32_t *) 0xe000ed20))
#define portNVIC_PENDSV_PRI (((uint32_t) configKERNEL_INTERRUPT_PRIORITY ) << 16UL)
#define portNVIC_SYSTICK_PRI (((uint32_t) configKERNEL_INTERRUPT_PRIORITY ) << 24UL )

/* SysTick ���ƼĴ��� */ 
#define portNVIC_SYSTICK_CTRL_REG (*((volatile uint32_t *) 0xe000e010 ))
/* SysTick ��װ�ؼĴ����Ĵ��� */
#define portNVIC_SYSTICK_LOAD_REG (*((volatile uint32_t *) 0xe000e014 ))
	
///* SysTick ʱ��Դѡ�� */
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
extern StackType_t *pxPortInitialiseStack( StackType_t *pxTopOfStack,     //ջ��ָ���ַ���ս�����ʱ��ջ�գ�ջ���ڸߵ�ַλ�ã�
									TaskFunction_t pxCode,         //������ڵ�ַ
									void *pvParameters );          //���ݵĲ����ĵ�ַ

void vPortSetupTimerInterrupt(void);
#endif 

