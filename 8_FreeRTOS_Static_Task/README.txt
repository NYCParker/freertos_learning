If the application only uses CMSIS libraries for interrupt configuration 
then the correct setting can be achieved on all Cortex-M devices by calling NVIC_SetPriorityGrouping( 0 ) before starting the scheduler.  
Note however that some vendor specific peripheral libraries assume a non-zero priority group setting, 
in which cases using a value of zero will result in unpredictable behaviour
