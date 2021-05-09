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

//��ʼ�������񿪱ٵ�ջ
StackType_t *pxPortInitialiseStack( StackType_t *pxTopOfStack,     //ջ��ָ���ַ���ս�����ʱ��ջ�գ�ջ���ڸߵ�ַλ�ã�
									TaskFunction_t pxCode,         //������ڵ�ַ
									void *pvParameters )           //���ݵĲ����ĵ�ַ
{
	pxTopOfStack--;
	*pxTopOfStack = portINITIAL_XPSR;    //����xPSR,������ֵΪportINITIAL_XPSR��0x01000000����ʵ����һ����ʼ״̬�����е�1��ʾThumb״̬����ΪCortex-Mֻ��Thumb״̬ XPSR
	pxTopOfStack--;
	*pxTopOfStack = ( ( StackType_t ) pxCode ) & portSTART_ADDRESS_MASK; //����������ڵ�ַ�����ֶ��루CPU�ɵ����ߣ�Ӧ���ǲ���©�ģ�PC 
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) prvTaskExitError; //���溯�����ص�ַ LR
	pxTopOfStack -= 5;                         //�����ռ䱣��Ĵ���R12 R3 R2 R1 
	*pxTopOfStack = (StackType_t)pvParameters;  //�������������ַ R0
	pxTopOfStack -= 8;            //����R4~R11��֮���ֶ����ص�CPU�Ĵ��������ݣ�
	
	return pxTopOfStack; //���ش�ʱ��ջ��ָ��
}

//vPortSVCHandlerҪ���ľ��ǰ�����ջR11-R4���ݸ���CPU�Ĵ���R11-R4����pvParameters ��ֵ��PSP�Ĵ�����
//����8���ֵ����ݣ�R0~XPSR�������˳��쳣ʱ�Զ����ص�CPU�Ĵ�����PCָ��Ҳָ����������ڵ�ַ�Ӷ���ת����һ������


//��ʼ��һ��������Ҫ��������������һ���Ǹ��� MSP ��ֵ�����ǲ��� SVC ϵͳ���ã�
//Ȼ��ȥ�� SVC ���жϷ��������������л�����һ������
__asm void prvStartFirstTask(void)
{
	PRESERVE8;            //��ǰջ����8�ֽڶ���
	
	ldr r0, =0xE000ED08   /* Use the NVIC offset register to locate the stack. */  //Cortext-M3Ӳ����,0xE000ED08��ַ��ΪVTOR(������ƫ����)�Ĵ���,�洢��������ʼ��ַ
	ldr r0, [r0]          //ȡ���������һ��������һ�������ջָ��MSP�ĳ�ʼֵ��Ҳ�������������ʼ��ַ����memory����ʼ��ַ��
	ldr r0, [r0]          //��ʼ��ַָ������ݼ��ص�r0
	
	msr msp, r0           //�� R0 ��ֵ�洢�� MSP����������ջ��ջ��ָ�롣��ʼ��һ�������е���࣬   //��ջ��ջ��ָ��
	                      //��Ϊ��ϵͳ������ʱ��ִ���� Reset_Handler��ʱ�� �������Ѿ���ʼ����ϣ� MSP ��ֵ���Ѿ�����Ϊ���������ʼֵ����ָ������ջ��ջ��ָ��
	                      // ���Է��ֲ�һ��
	
	cpsie i               //���ж�
	cpsie f               //���쳣
	dsb                   //����ͬ������
	isb	                  //ָ��ͬ������
	
	svc 0                 //����ϵͳ���ã������ 0��ʾ SVC �жϣ�����������ִ�� SVC �жϷ�����
	nop
	nop
}

//�ں����� SCB �� SHPR3 �Ĵ����������� SysTick �� PendSV ���쳣���ȼ�
//* Bits 31:24 PRI_15[7:0]: Priority of system handler 15, SysTick exception
//* Bits 23:16 PRI_14[7:0]: Priority of system handler 14, PendSV
//�����л�������PendSV�ж��н��еģ�
BaseType_t xPortStartScheduler(void)
{
	portNVIC_SYSPRI2_REG |= portNVIC_PENDSV_PRI;
	portNVIC_SYSPRI2_REG |= portNVIC_SYSTICK_PRI; //�����쳣�����ȼ���������� 63������λ��Ч��
	
	//������һ�������ٷ���
	uxCriticalNesting = 0;
	prvStartFirstTask();
	
	return 0;
}

//SVC�жϷ�����
__asm void vPortSVCHandler( void )
{
	extern pxCurrentTCB;    //pxCurrentTCBָ��ǰ�������л��߼���Ҫ���е������������ƿ�
	
	PRESERVE8
	ldr r3, =pxCurrentTCB 
	ldr r1, [r3] 
	ldr r0, [r1]            //����������ƿ鵽r0 ��������ƿ�Ӧ���Ǹոձ�������������ƿ��һ����Ա��ջ��ָ�룬r0��ʱ����ջ��ָ�룩
	ldmia r0!, {r4-r11}     //�� r0 Ϊ����ַ����ջ������������ 8 ���ֵ����ݼ��ص� CPU �Ĵ��� r4~r11������ʼ��������ջr4~r11��ֵ����cpu�Ĵ�����ͬʱr0������ͣ�������β�R0��
	msr psp, r0             //r0д��psp
	isb
	mov r0, #0              //����r0
	msr basepri, r0         //r0д��basepri �������ж�basepri ��һ���ж����μĴ��������ڵ��ڴ˼Ĵ���ֵ���ж϶���������
	
	                        //�����ȸ�r14���ض������ֵ���˳����ԣ���Ȼ��bx��ת��r14���쳣����
	orr r14, #0xd           //ͨ���� r14 �Ĵ������ 4 λ��λ����0x0D��ʹ��Ӳ�����˳�ʱʹ�ý��̶�ջָ�� PSP ��ɳ�ջ���������غ��������ģʽ������Thumb ״̬
	bx r14                  //�쳣���أ��Զ�����ջ��ʣ�µ����ݵ�CPU�Ĵ�����PSPֵҲ����ָ������ջ��ջ��
}


//PendSV�жϷ�����
__asm void xPortPendSVHandler( void )
{
	extern pxCurrentTCB;
	extern vTaskSwitchContext;
	
	PRESERVE8
	                //����PendSVC Handler��������������л������Զ���ջ��PSPҲ���Զ����£�λ��Ҫ���
	mrs r0, psp     //psp���ݴ���R0��û���ĸ����ָ����ֱ�Ӳ���PSP�����ջ������ֻ�ܽ���R0
	isb             //ָ��ͬ������,����ˮ��
	
	ldr r3, =pxCurrentTCB
	ldr r2, [r3]    //��ǰ���������TCBָ�����R2����û�л���
	
	stmdb r0!,{r4-r11} //�Ĵ���R4~R11���浽��ǰ����������ջ��ͬ�����¼Ĵ���R0����ʱpsp��parameter��r0ָջ����r4��Ӧ��λ�ã���
	
	str r0,[r2]  //r0���ݱ��浽��ǰ����TCB�ĵ�һ����Աջ����֮���л�������Ҫ��������ջ����������ݳ�ջ)
	
	stmdb sp!,{r3,r14} //R3��R14��ʱѹջ��*************������ϵͳ�У��ж�ʹ�õ�������ջ��ջָ��ʹ�õ��� MSP��ѹ������ջ������
	                   //���ú���ʱ�����ص�ַ�Զ����浽R14�У�����һ�����÷�����R14��ֵ�ᱻ���ǣ������Ҫ��ջ������
	                   //R3����ĵ�ǰ���������TCBָ��(pxCurrentTCB)��ַ���������ú���õ������ҲҪ��ջ����
	//��ʼ�л�
	
	mov r0, #configMAX_SYSCALL_INTERRUPT_PRIORITY 
    msr basepri, r0       //�����ٽ������ж����ȼ��Ŵ��ڵ���#configMAX_SYSCALL_INTERRUPT_PRIORITY���ж϶��ᱻ����
	dsb
	isb
	bl vTaskSwitchContext;
	
	mov r0, #0           
	msr basepri, r0  //��Ĵ���BASEPRIд����ֵ0���˳��ٽ��������ʱ��pxCurrentָ��ָ���������ƿ���ˣ��䵽���Ҫ��R3�п�����ΪR3������ָ��ĵ�ַ��
	
	ldmia sp!, {r3, r14} //���Ĵ���R3��R14������ջ�лָ�,R3����pxCurrentTCB��ַ��R14�����˳��쳣��Ҫ����Ϣ
	ldr r1, [r3]
	ldr r0, [r1]    //���������е������ջջ��ֵ����R0
	
	ldmia r0!, {r4-r11} //�����������ʼ��r4-r11��ջ
	msr psp, r0      //�����µ�ջ����ֵ���̶߳�ջָ��PSP
	isb
	bx r14           //��ջ
	nop
}

/* ==========�����ٽ�Σ� �����жϱ����汾������Ƕ��=============== */
//�����ٽ�Σ�û�ӱ��������˸����ԣ�
void vPortEnterCritical(void)
{
	portDISABLE_INTERRUPTS();
	uxCriticalNesting ++;
	
	if (uxCriticalNesting == 1)
	{
		configASSERT( ( portNVIC_INT_CTRL_REG & portVECTACTIVE_MASK) == 0);
	}
}

//��������Ҳ�ж�Ӧ�ĺ궨��
/* ==========�����ٽ�Σ����жϱ����汾������Ƕ��=============== */
//������portmacro.h   portSET_INTERRUPT_MASK_FROM_ISR

/* ==========�˳��ٽ�Σ������жϱ����汾������Ƕ��=============== */
void vPortExitCritical( void )
{
	configASSERT( uxCriticalNesting );
	uxCriticalNesting--;
	if ( uxCriticalNesting == 0 )
	{
		portENABLE_INTERRUPTS();
	}
}

/* ==========�˳��ٽ�Σ����жϱ����汾������Ƕ��=============== */
//������portmacro.h   portCLEAR_INTERRUPT_MASK_FROM_ISR( x ) 

