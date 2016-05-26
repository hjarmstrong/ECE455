#include <lpc17xx.h>
#include "states.h" 

void TIMER0_IRQHandler(void) 
{	
	// Acknowledge the Interupt
  LPC_TIM0->IR |= 1;
	
	__disable_irq();
	LPC_TIM0->TCR = (1<<1);

	transition(&input, 't');
	
	// The Timer is no longer required, disable it and reset the reset
	LPC_TIM0->TCR = 0;
	
	__enable_irq();
}


void timerInit(void)
{	
  // Initially, the clock goes from 0 -> 1 and interupts, we want
	// it to start as 1 and go from 1 -> 0 -> 1 and then interupt/
  LPC_TIM0->TC = 1;
	
  // The TC ticks: 1 -> 0 -> 1, then interupts, so the prescale 
  // Timer must count half of one second, which at 25Mhz, is: 
  // (25 - 1) M /2 = 12499999 prescale ticks. because 1 tick
  // occurs when TC ticks.
  LPC_TIM0->PR = 12599999;
	//LPC_TIM0->PR = 0x21;

  LPC_TIM0->MR0 = 1;
  
	// Set TC to 0 when it reaches 1, and interupt when TC reaches MR0
  LPC_TIM0->MCR = 1 | (1 << 1);

  NVIC_EnableIRQ(TIMER0_IRQn);

  // Finally, we enable the Timer
  // LPC_TIM0->TCR = 1;
}
