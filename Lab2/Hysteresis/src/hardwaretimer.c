#include <lpc17xx.h>
#include "System/GLCD.h"
#include "states.h" 
#include "timer.h"

void TIMER0_IRQHandler(void) 
{	
	// Acknowledge the Interupt
  LPC_TIM0->IR |= 1;
	
	__disable_irq();
	
	LPC_TIM0->TCR = 0; // Diable Debounce timer

	if(input.currState == Debounce)
	{
		// Assert button is low (high logically but pressing it makes it go low)
		timerInit(12599999); // start dash timer
	}
	else if (input.currState == DotDebounce)
	{
		// Assert button is high
		//GLCD_DisplayString(3, 0, 1, "Dot");
		//transition(&morse, '.');
	}
	else if (input.currState == DashDebounce)
	{
		// Assert button is high
   // GLCD_DisplayString(3, 0, 1, "Dash");
    //transition(&morse, '-');
	}

	transition(&input, 't');
	
	
	__enable_irq();
}


void timerInit(int cycles)
{	
  LPC_TIM0->TCR = (1<<1);
  
	// The TC ticks: 1 -> 0 -> 1, then interupts, so the prescale 
  // Timer must count half of one second, which at 25Mhz, is: 
  // (25 - 1) M /2 = 12499999 prescale ticks. because 1 tick
  // occurs when TC ticks.
  LPC_TIM0->PR = cycles;
	//LPC_TIM0->PR = 0x21;

  LPC_TIM0->MR0 = 1;
  
	// Set TC to 0 when it reaches 1, and interupt when TC reaches MR0
  LPC_TIM0->MCR = 1 | (1 << 1);


  // Finally, we enable the Timer
  LPC_TIM0->TCR = 1;
}
