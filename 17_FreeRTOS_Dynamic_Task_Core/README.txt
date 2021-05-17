If the application only uses CMSIS libraries for interrupt configuration 
then the correct setting can be achieved on all Cortex-M devices by calling NVIC_SetPriorityGrouping( 0 ) before starting the scheduler.  
Note however that some vendor specific peripheral libraries assume a non-zero priority group setting, 
in which cases using a value of zero will result in unpredictable behaviour

动态任务创建
 /* 如果这是第一次调用 malloc 
 那么堆将需要初始化， 以设置空闲块列表。 */
如果这是第一次调用 malloc 那么需要将堆进行初始化，以设置空闲块列表
方便以后分配内存，初始化完成之后会取得堆的结束地址，
在 MemMang 中的5个内存分配 heap_x.c 文件中实现


configSUPPORT_DYNAMIC_ALLOCATION 动态分配宏



