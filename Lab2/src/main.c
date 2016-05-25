#include <lpc17xx.h>
#include "System/GLCD.h"
#include "states.h" 
#include "timer.h"
#include "button.h"

int main(void)
{
  SystemInit();
	GLCD_Init();
	

	
	createFSM(morse, NUM_MORSE_STATES, Morse_Start);
	setTransition(morse, ".", Morse_Start, Dot);
	setTransition(morse, "-", Dot, DotDash);
	setTransition(morse, "-", DotDash, DotDashDash);
	setTransition(morse, ".", DotDashDash, DotDashDashDot);
	setTransition(morse, "-", DotDashDashDot, DotDashDashDotDash);
	setTransition(morse, ".", DotDashDashDotDash, DotDashDashDotDashDot);
	setTransition(morse, ".", DotDashDashDotDashDot, DotDashDashDotDashDotDot);
	
	createFSM(input, NUM_INPUT_STATES, Input_Start);
	setTransition(input, "p", Input_Start, InProgress);
	setTransition(input, "t", InProgress, OutputDot);
	setTransition(input, "p", InProgress, OutputDash);

	GLCD_Clear(White);
	GLCD_DisplayString(0, 0, 1, "Hello World");
	
	__disable_irq();
	timerInit();
	buttonInit();
	__enable_irq();
	
	while(1)
	{
		if(morse.currState == DotDashDashDotDashDotDot)
		{
			GLCD_DisplayString(0, 0, 1, "Correct");
		}
		else if(morse.currState == Morse_NUL)
		{
			// We recieved something invalid, start over
      resetFSM(morse);
		}
		
		if(input.currState == OutputDot)
		{
			transition(morse, '.');
			
			// Make sure we always keep track of when it might
      // be the start of the correct pattern.
			if(morse.currState == Morse_NUL)
		  {
        resetFSM(morse);
				transition(morse, '.');
		  }
			
			resetFSM(input);
		}
		
		if(input.currState == OutputDash)
		{
			transition(morse, '-');
			resetFSM(input);
		}
  }
}
