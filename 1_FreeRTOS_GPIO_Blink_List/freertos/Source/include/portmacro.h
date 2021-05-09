
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

#endif /* PORTMACRO_H */
