#include <XMC4400.h>
#include "User_Func.h"
#include "Var_Init.h"
#include "stdio.h"

void UART_Agreement(void);

//д����һ�����������ô���ָ����水ť������ͻȻ���ֲ��������ж��н��й��𣬻ָ���ɾ���Ȳ���
//���һ�������ԼӸ�����������������

void UART_Agreement(void)
{
	if(Receive_Data[0] == 0x31)
	{
		printf("�յ�ָ��1\n"); 
		Task_Cmd = 1;
	}
	
	if(Receive_Data[0] == 0x32)
	{
		printf("�յ�ָ��2\n");
        Task_Cmd = 2;		
	}
}
