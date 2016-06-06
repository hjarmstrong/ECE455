#include <lpc17xx.h>
#include "System/LCDAssert.h"
#include "adc.h"

int selectedTemp = -3;
int realTemp;

void ADC_IRQHandler(void) 
{
	realTemp = ((LPC_ADC->ADGDR>>4) & 0xFFF) / 80;
	realTemp -= 12;
	
	LPC_ADC->ADCR |= (1<<24);
}

void ADCInit(void)
{
	LPC_PINCON->PINSEL1 &= ~(3<<18);
	LPC_PINCON->PINSEL1 |= (1<<18);
	
	LPC_SC->PCONP |= (1<<12);
	
	LPC_ADC->ADCR = (1<<2) | (4<<8) | (1<<21);
	
	LPC_ADC->ADCR |= (1<<24);
	
	// poll for first value because system can
	// not start without an initial temperature.
	//while ((LPC_ADC->ADGDR & 0x8000) == 0);
	
	//realTemp = (LPC_ADC->ADGDR>>4) & 0xFFF;
		
	// Enable interupts to read the temp constantly
	LPC_ADC->ADINTEN = (1<<8);
	NVIC_EnableIRQ(ADC_IRQn);
}
