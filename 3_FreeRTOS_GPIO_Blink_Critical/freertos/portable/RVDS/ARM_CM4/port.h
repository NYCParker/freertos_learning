#ifndef PORT_H
#define PORT_H

#include "list.h"
#include "projdefs.h"

#define portNVIC_SYSPRI2_REG (*(( volatile uint32_t *) 0xe000ed20))
#define portNVIC_PENDSV_PRI (((uint32_t) configKERNEL_INTERRUPT_PRIORITY ) << 16UL)
#define portNVIC_SYSTICK_PRI (((uint32_t) configKERNEL_INTERRUPT_PRIORITY ) << 24UL )

extern StackType_t *pxPortInitialiseStack( StackType_t *pxTopOfStack,     //ջ��ָ���ַ���ս�����ʱ��ջ�գ�ջ���ڸߵ�ַλ�ã�
									TaskFunction_t pxCode,         //������ڵ�ַ
									void *pvParameters );          //���ݵĲ����ĵ�ַ
#endif 

