
/*
中断处理函数
*/

#include <XMC4400.h>
#include "Irq_Handle.h"
#include <xmc_gpio.h>
#include "GPIO.h"
#include "Var_Init.h"
#include "User_Func.h"

void IRQ_Enable(void)
{
    NVIC_SetPriority(USIC0_0_IRQn, 1);
	NVIC_EnableIRQ(USIC0_0_IRQn);
}

//串口中断
void USIC0_0_IRQHandler(void)
{
	if(RD_REG(USIC0_CH1->PSR, USIC_CH_PSR_RIF_Msk, USIC_CH_PSR_RIF_Pos) == 1)		              //判断接收中断标志
	{	
		SET_BIT(USIC0_CH1->PSCR, USIC_CH_PSCR_CRIF_Pos);
		XMC_GPIO_ToggleOutput(GPIO_LED1);
		
		Receive_Data[Receive_Count] = USIC0_CH1->RBUF;     //Receive_Data for debug 
		UART_Agreement();
		
		Receive_Count++;
		if(Receive_Count == REC_LENGTH)
		{
			Receive_Count = 0;
		}	
	}
	
	if(RD_REG(USIC0_CH1->PSR, USIC_CH_PSR_AIF_Msk, USIC_CH_PSR_AIF_Pos) == 1)		              //判断备选接收中断标志
	{	
		SET_BIT(USIC0_CH1->PSCR, USIC_CH_PSCR_CAIF_Pos);
		XMC_GPIO_ToggleOutput(GPIO_LED2);
	}	
}


