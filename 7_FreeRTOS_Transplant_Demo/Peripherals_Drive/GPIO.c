/*
GPIO 
P5.2 LED2
P1.8 LED1
*/

#include <XMC4400.h>
#include "GPIO.h"
#include <xmc_gpio.h>

void GPIO_Init(void)
{
	XMC_GPIO_CONFIG_t config;
	
	config.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL;        
	XMC_GPIO_Init(GPIO_LED1, &config);             //P1.8��ͨ���
	XMC_GPIO_Init(GPIO_LED2, &config);             //P5.2��ͨ���
	
	config.mode = XMC_GPIO_MODE_INPUT_PULL_UP;           //RX��������
	XMC_GPIO_Init(GPIO_RX, &config);
	
	config.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT2;   //TX����UART���U0CH1(Ҫ���죩
	XMC_GPIO_Init(GPIO_TX, &config);
}


