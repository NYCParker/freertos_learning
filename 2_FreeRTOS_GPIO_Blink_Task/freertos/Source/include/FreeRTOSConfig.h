/*�� FreeRTOS �У����������ʱ��������ѱ��������͵���ǰ׺���ڱ����ϣ� ������
�ô������û�һ�������������֪���ñ��������͡� 
���� char �ͱ�����ǰ׺�� c�� 
short �ͱ�����ǰ׺�� s�� 
long �ͱ�����ǰ׺�� l�� 
portBASE_TYPE ���ͱ�����ǰ׺�� x�� 
�����������������ͣ��������ݽṹ�������������о���ȶ���ı�������ǰ׺Ҳ�� x��
���һ���������޷����͵���ô����һ��ǰ׺ u�� 
�����һ��ָ����������һ��ǰ׺ p
��ˣ������Ƕ���һ���޷��ŵ� char �ͱ�����ʱ����һ�� uc ǰ׺��
������һ��char �͵�ָ�������ʱ�����һ�� pc ǰ׺*/

#define xPortPendSVHandler PendSV_Handler
#define xPortSysTickHandler SysTick_Handler
#define vPortSVCHandler SVC_Handler

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#define configUSE_16_BIT_TICKS 0
#define configMAX_TASK_NAME_LEN 16
#define configSUPPORT_STATIC_ALLOCATION 1
#define configMAX_PRIORITIES 5

#define configPRIO_BITS       		6 
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY			0x3f
#define configKERNEL_INTERRUPT_PRIORITY 		( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )

#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY	5 
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )

#endif /* FREERTOS_CONFIG_H */
