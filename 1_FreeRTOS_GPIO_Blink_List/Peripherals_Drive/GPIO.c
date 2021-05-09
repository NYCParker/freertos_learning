/*
GPIO 
P5.2 LED1
*/

#include <XMC4400.h>
#include "GPIO.h"
#include <xmc_gpio.h>

void GPIO_Init(void)
{
	XMC_GPIO_CONFIG_t config;
	
	config.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL;        
	XMC_GPIO_Init(GPIO_LED, &config);             //P1.8ÆÕÍ¨Êä³ö
}
