#include <lpc17xx.h>
#include "System/LCDAssert.h"
#include "System/GLCD.h"
#include "states.h"
#include "button.h"

int main(void)
{
  SystemInit();
	GLCD_Init();
	
	GLCD_Clear(White);

	__disable_irq();
	NVIC_EnableIRQ(TIMER0_IRQn);
	
	buttonInit();

	// Initilize LEDs
	// Set direction to output
  LPC_GPIO1->FIODIR |= 0xB0000000;
  LPC_GPIO2->FIODIR |= 0x0000007C;	
	
	__enable_irq();
	
	GLCD_DisplayString(0, 0, 1, "Interupts On ");
	
	// ---- Button Management FSM ----
	createFSM(&input, NUM_INPUT_STATES, Input_Start); 
	setTransition(&input, "p", Input_Start, Debounce);
	setTransition(&input, "p", Debounce, Debounce);
	setTransition(&input, "t", Debounce, InProgress);
	
	setTransition(&input, "p", InProgress, Done);
	setTransition(&input, "p", Done, Done);
	setTransition(&input, "t", Done, Input_Start);
	// Where p is a press or release of INT0, and t is a timer interupt
	
	while(1)
	{
		if(input.currState == 0)
		{
			LCD_ASSERT(0);
		}
	}
}
