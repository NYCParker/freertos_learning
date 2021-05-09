
#ifndef PORTMACRO_H
#define PORTMACRO_H

#include "stdint.h"   //<stdint.h>�ж����˼�����չ���������ͺͺ�
#include "stddef.h"   //stddef.h������һЩ��׼���Լ�����.   NULL ��ָ�볣��
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
	#define portMAX_DELAY ( TickType_t ) 0xffffffffUL     //��configUSE_16_BIT_TICKSĬ��Ϊ0 TickType_t��ʾ32λ uint32
	#endif
	
#define portNVIC_INT_CTRL_REG (*(( volatile uint32_t *) 0xe000ed04)) 
#define portNVIC_PENDSVSET_BIT ( 1UL << 28UL )	
#define portSY_FULL_READ_WRITE ( 15 )

#define portYIELD() \
{ \
/* ���� PendSV�������������л� */ \
portNVIC_INT_CTRL_REG = portNVIC_PENDSVSET_BIT; \
__dsb( portSY_FULL_READ_WRITE ); \
__isb( portSY_FULL_READ_WRITE ); \
}

//portYIELDʵ�ʾ��ǽ� PendSV ������λ�� 1����û�������ж����е�ʱ����Ӧ PendSV �ж�(���ȼ���ͣ���
//ȥִ������д�õ� PendSV�жϷ�������������ʵ�������л�
//�����л��ı�����ͨ������ PendSV �жϣ����ж���ִ�������л�

#endif /* PORTMACRO_H */
