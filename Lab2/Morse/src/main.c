#include <lpc17xx.h>
#include "System/LCDAssert.h"
#include "System/GLCD.h"
#include "states.h" 
#include "timer.h"
#include "button.h"

int main(void)
{
  SystemInit();
	GLCD_Init();
	
	GLCD_Clear(White);
	
	// ---- Morse Code FSM ----

	createFSM(&morse, NUM_MORSE_STATES, Morse_Start);
	setTransition(&morse, ".", Morse_Start, Dot);
	setTransition(&morse, "-", Morse_Start, Morse_Start);
	
	setTransition(&morse, "-", Dot, DotDash);
	setTransition(&morse, ".", Dot, Dot);
	
	setTransition(&morse, ".", DotDash, Dot);
	setTransition(&morse, "-", DotDash, DotDashDash);
	
	setTransition(&morse, ".", DotDashDash, DotDashDashDot);
	setTransition(&morse, "-", DotDashDash, Morse_Start);
	
	setTransition(&morse, "-", DotDashDashDot, DotDashDashDotDash);
	setTransition(&morse, ".", DotDashDashDot, Dot);
	
	setTransition(&morse, ".", DotDashDashDotDash, DotDashDashDotDashDot);
	setTransition(&morse, "-", DotDashDashDotDash, DotDashDash);
	
	setTransition(&morse, ".", DotDashDashDotDashDot, DotDashDashDotDashDotDot);
	setTransition(&morse, "-", DotDashDashDotDashDot, DotDash);
	
	// Once the correct pattern is found, we are done, no more transitions
	setTransition(&morse, ".", DotDashDashDotDashDotDot, DotDashDashDotDashDotDot);
	setTransition(&morse, "-", DotDashDashDotDashDotDot, DotDashDashDotDashDotDot);
	
	// ---- Button Management FSM ----
 
	createFSM(&input, NUM_INPUT_STATES, Input_Start); 
	setTransition(&input, "p", Input_Start, Debounce);
	setTransition(&input, "p", Debounce, Debounce);
	setTransition(&input, "t", Debounce, InProgress);
	
	setTransition(&input, "p", InProgress, DotDebounce);
	setTransition(&input, "p", DotDebounce, DotDebounce);
	setTransition(&input, "t", DotDebounce, Input_Start);
	
	setTransition(&input, "t", InProgress, IsDash);
	setTransition(&input, "p", IsDash, DashDebounce);
	setTransition(&input, "p", DashDebounce, DashDebounce);
	setTransition(&input, "t", DashDebounce, Input_Start);
	// Where p is a press or release of INT0, and t is a timer interupt

		
	__disable_irq();
	NVIC_EnableIRQ(TIMER0_IRQn);
	buttonInit();
	__enable_irq();
	
	while(1)
	{
		if(morse.currState == DotDashDashDotDashDotDot)
		{
			GLCD_Clear(White);
			GLCD_DisplayString(4, 0, 1, "Correct");
			return 0;
		}
		else if((morse.currState == Morse_NUL) || (input.currState == Input_NUL))
		{
			//Something was wrong with the program, or the assumptions leading up to this point
			LCD_ASSERT(0);
		}
  }
}
