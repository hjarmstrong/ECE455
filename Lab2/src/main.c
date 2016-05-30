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
	
	createFSM(&input, NUM_INPUT_STATES, Input_Start); // If statments:
	setTransition(&input, "p", Input_Start, Debounce); // In button ISR, (send p, if(state == debounce || DotDebounce || DashDebounce) {set debounce timer}
	setTransition(&input, "p", Debounce, Debounce);  // In button ISR, (send p, if(state == debounce) {set debounce timer})
	setTransition(&input, "t", Debounce, InProgress); // In timer ISR, (if(state == debounce) {disable debounce timer, start dash timer}, send t)
	setTransition(&input, "p", InProgress, DotDebounce); // In button ISR set debounce timer agian, just like above
	setTransition(&input, "p", DotDebounce, DotDebounce);
	setTransition(&input, "t", DotDebounce, Input_Start); //In timer ISR
	
	setTransition(&input, "t", InProgress, IsDash);
	setTransition(&input, "p", IsDash, DashDebounce);
	setTransition(&input, "p", DashDebounce, DashDebounce);
	setTransition(&input, "t", DashDebounce, Input_Start); //In timer ISR (state == dash debounce){send dash}, send t)

		
	__disable_irq();
	NVIC_EnableIRQ(TIMER0_IRQn);
	buttonInit();
	__enable_irq();
	
	while(1)
	{
		if(morse.currState == DotDashDashDotDashDotDot)
		{
			GLCD_DisplayString(0, 0, 1, "Correct");
			return 0;
			__disable_irq();
			while(1) {}
		}
		else if((morse.currState == Morse_NUL) || (input.currState == Input_NUL))
		{
			//Something was wrong with the program, or the assumptions leading up to this point
			LCD_ASSERT(0);
		}
		
  }
}
