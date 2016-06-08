#include <lpc17xx.h>
#include "System/GLCD.h"
#include "displaystate.h"
#include "states.h" 
#include "timer.h"

void TIMER0_IRQHandler(void) 
{	
	// Acknowledge the Interupt
  LPC_TIM0->IR |= 1;
	
	__disable_irq();
	
	// Diable The Timer
	LPC_TIM0->TCR = 0; 

	if(input.currState == Debounce)
	{
		// start dash timer for 0.504 seconds
		timerInit(12599999);
	}
	else if (input.currState == DotDebounce)
	{
		DisplayState("Dot ");
		transition(&morse, '.');
	}
	else if (input.currState == DashDebounce)
	{
    DisplayState("Dash");
    transition(&morse, '-');
	}

	transition(&input, 't');
	
	
	__enable_irq();
}


void timerInit(int cycles)
{	
	// Reset The Timer
  LPC_TIM0->TCR = (1<<1);
  
	// The PC counts cycles, and ticks the TC when
	// it reache sthe values stored in PR
  LPC_TIM0->PR = cycles;

  LPC_TIM0->MR0 = 1;
  
	// Set TC to 0 when it reaches 1, and interupt
	// when TC reaches MR0
  LPC_TIM0->MCR = 1 | (1 << 1);

  // Finally, we enable the Timer, and clear the reset bit
  LPC_TIM0->TCR = 1;
}
