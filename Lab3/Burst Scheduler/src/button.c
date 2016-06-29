#include <lpc17xx.h>
#include "states.h"
#include "System/LCDAssert.h"
#include "timer.h"

void EINT3_IRQHandler(void) 
{
	// Acknowledge the Interupt
  LPC_GPIOINT->IO2IntClr |= (1<<10);
	
	__disable_irq();
	
	transition(&input, 'p');
	
	// Turn on the debounceing timer after every edge
	// After 5ms with a constant level we assume the 
	// pin is stable.
	timerInit(125000);
		
	__enable_irq();
}

void buttonInit(void)
{
	// Set the INT0 button's pin to GPIO, and configure it
	// as an input.
	LPC_PINCON->PINSEL4 &= ~(3<<20);
	LPC_GPIO2->FIODIR &= ~(1<<10);
	
	// We want interupts on both riseing and falling edges
	// in order to record both presses and releases.
	LPC_GPIOINT->IO2IntEnF |= (1<<10);
	LPC_GPIOINT->IO2IntEnR |= (1<<10);
	
	// Finally, we enable the inturupt
	NVIC_EnableIRQ(EINT3_IRQn);
}

