#include <lpc17xx.h>
#include "System/LCDAssert.h"
#include "adc.h"

void ADCInit(void)
{
	LPC_PINCON->PINSEL1 &= ~(3<<18);
	LPC_PINCON->PINSEL1 |= (1<<18);
	
	LPC_SC->PCONP |= (1<<12);
	
	LPC_ADC->ADCR = (1<<2) | (4<<8) | (1<<21);
}
