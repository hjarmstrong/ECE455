#include <lpc17xx.h>
#include <stdio.h>
#include "System/GLCD.h"
#include "states.h" 
#include "timer.h"
#include "adc.h"

void TIMER0_IRQHandler(void) 
{	
	unsigned char s[] = "-XX";
	// Acknowledge the Interupt
  LPC_TIM0->IR |= 1;
	
	__disable_irq();
	
	// Diable Debounce timer
	LPC_TIM0->TCR = 0; 

	if(input.currState == Debounce)
	{
		 // start dash timer
		timerInit(12599999);
	}
	else if (input.currState == DotDebounce)
	{
		selectedTemp++;
		sprintf((char *)s, "%03d", selectedTemp);
		GLCD_DisplayString(3, 3, 1, s);
	}
	else if (input.currState == DashDebounce)
	{
		selectedTemp--;
		sprintf((char *)s, "%03d", selectedTemp);
		GLCD_DisplayString(3, 3, 1, s);
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
