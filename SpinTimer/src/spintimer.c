#include <lpc17xx.h>

void delay(int ms)
{
	__disable_irq();
	for(volatile int i = 0; i < ms; i++)
  {
		for(volatile int j = 0; j < 20000; j++) // Seems pretty correct...
		{
		}
  }
  __enable_irq();	
}
