#include <lpc17xx.h>
#include "System/LCDAssert.h"
#include "System/GLCD.h"
#include "states.h" 
#include "timer.h"
#include "button.h"
#include "adc.h"

int main(void)
{
  SystemInit();
	GLCD_Init();
		
	GLCD_Clear(White);

	// Heading Labels
	GLCD_DisplayString(0, 1, 1, "-- Temperatures --");
	GLCD_DisplayString(2, 3, 1, "Set");
	GLCD_DisplayString(2, 8, 1, "Actual");
	
	// The initial set temperature is -3
	GLCD_DisplayString(3, 3, 1, "-03");
	
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

	createFSM(&furnace, NUM_FURNACE_STATES, FurnaceOff);
	setTransition(&furnace, "h", FurnaceOn, FurnaceOff);
	setTransition(&furnace, "c", FurnaceOff, FurnaceOn);
	setTransition(&furnace, "h", FurnaceOff, FurnaceOff);
	setTransition(&furnace, "c", FurnaceOn, FurnaceOn);
	
	__disable_irq();
	NVIC_EnableIRQ(TIMER0_IRQn);
	buttonInit();
	ADCInit();
	
	// Initilize LEDs
	// Set direction to output
  LPC_GPIO1->FIODIR |= 0xB0000000;
  LPC_GPIO2->FIODIR |= 0x0000007C;	
	
	__enable_irq();
	
	while(1)
	{
		if((input.currState == Input_NUL) || (furnace.currState == Input_NUL))
		{
			//Something was wrong with the program, or the assumptions leading up to this point
			LCD_ASSERT(0);
		}
		
  }
}
