#include <XMC4400.h>
#include "User_Func.h"
#include "Var_Init.h"
#include "stdio.h"

void UART_Agreement(void);

//写到这一步，本来想用串口指令代替按钮来挂起，突然发现不建议在中断中进行挂起，恢复，删除等操作
//灵机一动：可以加个变量，放在任务里

void UART_Agreement(void)
{
	if(Receive_Data[0] == 0x31)
	{
		printf("收到指令1\n"); 
		Task_Cmd = 1;
	}
	
	if(Receive_Data[0] == 0x32)
	{
		printf("收到指令2\n");
        Task_Cmd = 2;		
	}
}
