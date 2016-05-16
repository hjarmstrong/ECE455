#include <lpc17xx.h>

void timerInit(void)
{
	// nop for spinning
}

void delay(int ms)
{
	__disable_irq();
	for(volatile int i = 0; i < ms; i++)
  {
		// Assumes 5 clock cycles to exicute the loop
		// At 100Mhz 5 cycles => 20000000 per second, and 20000 per ms
		for(volatile int j = 0; j < 20000; j++) 
		{
		}
  }
  __enable_irq();	
}
