#include "port.h"
#include "Task.h"

#define portINITIAL_XPSR ( 0x01000000 )
#define portSTART_ADDRESS_MASK ( ( StackType_t ) 0xfffffffeUL )

static UBaseType_t uxCriticalNesting = 0xaaaaaaaa;

/* Masks off all bits but the VECTACTIVE bits in the ICSR register. */
#define portVECTACTIVE_MASK                   ( 0xFFUL )

static void prvTaskExitError(void)
{
	for(;;);         
}

//初始化给任务开辟的栈
StackType_t *pxPortInitialiseStack( StackType_t *pxTopOfStack,     //栈顶指针地址（刚进来的时候栈空，栈顶在高地址位置）
									TaskFunction_t pxCode,         //任务入口地址
									void *pvParameters )           //传递的参数的地址
{
	pxTopOfStack--;
	*pxTopOfStack = portINITIAL_XPSR;    //保存xPSR,且它的值为portINITIAL_XPSR，0x01000000。其实就是一个初始状态，其中的1表示Thumb状态。因为Cortex-M只有Thumb状态 XPSR
	pxTopOfStack--;
	*pxTopOfStack = ( ( StackType_t ) pxCode ) & portSTART_ADDRESS_MASK; //保存任务入口地址，半字对齐（CPU由低往高，应该是不会漏的）PC 
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) prvTaskExitError; //保存函数返回地址 LR
	pxTopOfStack -= 5;                         //留出空间保存寄存器R12 R3 R2 R1 
	*pxTopOfStack = (StackType_t)pvParameters;  //保存任务参数地址 R0
	pxTopOfStack -= 8;            //保存R4~R11（之后手动加载到CPU寄存器的内容）
	
	return pxTopOfStack; //返回此时的栈顶指针
}

//vPortSVCHandler要做的就是把任务栈R11-R4内容赋给CPU寄存器R11-R4，把pvParameters 赋值给PSP寄存器。
//另外8个字的内容（R0~XPSR）会在退出异常时自动加载到CPU寄存器，PC指针也指向了任务入口地址从而跳转到第一个任务


//开始第一个任务，主要做了两个动作，一个是更新 MSP 的值，二是产生 SVC 系统调用，
//然后去到 SVC 的中断服务函数里面真正切换到第一个任务
__asm void prvStartFirstTask(void)
{
	PRESERVE8;            //当前栈按照8字节对齐
	
	ldr r0, =0xE000ED08   /* Use the NVIC offset register to locate the stack. */  //Cortext-M3硬件中,0xE000ED08地址处为VTOR(向量表偏移量)寄存器,存储向量表起始地址
	ldr r0, [r0]          //取出向量表第一项，向量表第一项储存主堆栈指针MSP的初始值（也就是向量表的起始地址）（memory的起始地址）
	ldr r0, [r0]          //起始地址指向的内容加载到r0
	
	msr msp, r0           //将 R0 的值存储到 MSP，这是主堆栈的栈顶指针。起始这一步操作有点多余，   //主栈的栈顶指针
	                      //因为当系统启动的时候，执行完 Reset_Handler的时候， 向量表已经初始化完毕， MSP 的值就已经更新为向量表的起始值，即指向主堆栈的栈顶指针
	                      // 测试发现不一样
	
	cpsie i               //开中断
	cpsie f               //开异常
	dsb                   //数据同步隔离
	isb	                  //指令同步隔离
	
	svc 0                 //产生系统调用，服务号 0表示 SVC 中断，接下来将会执行 SVC 中断服务函数
	nop
	nop
}

//内核外设 SCB 中 SHPR3 寄存器用于设置 SysTick 和 PendSV 的异常优先级
//* Bits 31:24 PRI_15[7:0]: Priority of system handler 15, SysTick exception
//* Bits 23:16 PRI_14[7:0]: Priority of system handler 14, PendSV
//任务切换都是在PendSV中断中进行的，
BaseType_t xPortStartScheduler(void)
{
	portNVIC_SYSPRI2_REG |= portNVIC_PENDSV_PRI;
	portNVIC_SYSPRI2_REG |= portNVIC_SYSTICK_PRI; //两个异常的优先级都设置最低 63（高六位有效）
	
	//启动第一个任务不再返回
	uxCriticalNesting = 0;
	prvStartFirstTask();
	
	return 0;
}

//SVC中断服务函数
__asm void vPortSVCHandler( void )
{
	extern pxCurrentTCB;    //pxCurrentTCB指向当前正在运行或者即将要运行的任务的任务控制块
	
	PRESERVE8
	ldr r3, =pxCurrentTCB 
	ldr r1, [r3] 
	ldr r0, [r1]            //加载任务控制块到r0 （任务控制块应该是刚刚被创建，任务控制块第一个成员是栈顶指针，r0此时就是栈顶指针）
	ldmia r0!, {r4-r11}     //以 r0 为基地址，将栈中向上增长的 8 个字的内容加载到 CPU 寄存器 r4~r11，（起始就是任务栈r4~r11的值存入cpu寄存器）同时r0自增（停在任务形参R0）
	msr psp, r0             //r0写入psp
	isb
	mov r0, #0              //清零r0
	msr basepri, r0         //r0写入basepri 打开所有中断basepri 是一个中断屏蔽寄存器，大于等于此寄存器值的中断都将被屏蔽
	
	                        //下面先给r14赋特定意义的值（退出属性），然后bx跳转到r14，异常返回
	orr r14, #0xd           //通过向 r14 寄存器最后 4 位按位或上0x0D，使得硬件在退出时使用进程堆栈指针 PSP 完成出栈操作并返回后进入任务模式、返回Thumb 状态
	bx r14                  //异常返回，自动加载栈中剩下的内容到CPU寄存器，PSP值也更新指向任务栈的栈顶
}


//PendSV中断服务函数
__asm void xPortPendSVHandler( void )
{
	extern pxCurrentTCB;
	extern vTaskSwitchContext;
	
	PRESERVE8
	                //进入PendSVC Handler，激活任务的运行环境会自动入栈，PSP也会自动更新，位置要清楚
	mrs r0, psp     //psp内容存入R0，没有哪个汇编指令能直接操作PSP完成入栈，所以只能借助R0
	isb             //指令同步隔离,清流水线
	
	ldr r3, =pxCurrentTCB
	ldr r2, [r3]    //当前激活的任务TCB指针存入R2（还没切换）
	
	stmdb r0!,{r4-r11} //寄存器R4~R11保存到当前激活的任务堆栈，同步更新寄存器R0（此时psp在parameter，r0指栈顶（r4对应的位置））
	
	str r0,[r2]  //r0内容保存到当前任务TCB的第一个成员栈顶（之后切换回来还要从这里获得栈顶，完成数据出栈)
	
	stmdb sp!,{r3,r14} //R3和R14临时压栈（*************在整个系统中，中断使用的是主堆栈，栈指针使用的是 MSP，压入主堆栈！！）
	                   //调用函数时，返回地址自动保存到R14中，所以一旦调用发生，R14的值会被覆盖，因此需要入栈保护。
	                   //R3保存的当前激活的任务TCB指针(pxCurrentTCB)地址，函数调用后会用到，因此也要入栈保护
	//开始切换
	
	mov r0, #configMAX_SYSCALL_INTERRUPT_PRIORITY 
    msr basepri, r0       //进入临界区，中断优先级号大于等于#configMAX_SYSCALL_INTERRUPT_PRIORITY的中断都会被屏蔽
	dsb
	isb
	bl vTaskSwitchContext;
	
	mov r0, #0           
	msr basepri, r0  //向寄存器BASEPRI写入数值0来退出临界区（这个时候pxCurrent指针指向的任务控制块变了，变到哪里，要从R3中看，因为R3保留了指针的地址）
	
	ldmia sp!, {r3, r14} //将寄存器R3和R14从主堆栈中恢复,R3保存pxCurrentTCB地址，R14保存退出异常需要的信息
	ldr r1, [r3]
	ldr r0, [r1]    //将即将运行的任务堆栈栈顶值存入R0
	
	ldmia r0!, {r4-r11} //类比上面任务开始，r4-r11出栈
	msr psp, r0      //将最新的栈顶赋值给线程堆栈指针PSP
	isb
	bx r14           //出栈
	nop
}

/* ==========进入临界段， 不带中断保护版本，不能嵌套=============== */
//进入临界段，没加保护，加了个断言，
void vPortEnterCritical(void)
{
	portDISABLE_INTERRUPTS();
	uxCriticalNesting ++;
	
	if (uxCriticalNesting == 1)
	{
		configASSERT( ( portNVIC_INT_CTRL_REG & portVECTACTIVE_MASK) == 0);
	}
}

//在任务中也有对应的宏定义
/* ==========进入临界段，带中断保护版本，可以嵌套=============== */
//定义在portmacro.h   portSET_INTERRUPT_MASK_FROM_ISR

/* ==========退出临界段，不带中断保护版本，不能嵌套=============== */
void vPortExitCritical( void )
{
	configASSERT( uxCriticalNesting );
	uxCriticalNesting--;
	if ( uxCriticalNesting == 0 )
	{
		portENABLE_INTERRUPTS();
	}
}

/* ==========退出临界段，带中断保护版本，可以嵌套=============== */
//定义在portmacro.h   portCLEAR_INTERRUPT_MASK_FROM_ISR( x ) 

