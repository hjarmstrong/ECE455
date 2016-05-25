#include <lpc17xx.h>
#include "states.h"

void EINT3_IRQHandler(void) 
{
	// Acknowledge the Interupt
  LPC_GPIOINT->IO2IntClr |= (1<<10);
	
	__disable_irq();
	
	transition(&input, 'p');
	
	// If the timer is not already on, turn it on
	if(!(LPC_TIM0->TCR & 1))
	{
     LPC_TIM0->TCR = 1;
	}
	
	__enable_irq();

}

void buttonInit(void)
{
	LPC_PINCON->PINSEL4 &= ~(3<<20);
	LPC_GPIO2->FIODIR &= ~(1<<10);
	
	LPC_GPIOINT->IO2IntEnF |= (1<<10);
	NVIC_EnableIRQ(EINT3_IRQn);
}

