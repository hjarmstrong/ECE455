#include <lpc17xx.h>
#include "System/GLCD.h"
#include "timer.h"

int main(void)
{
  SystemInit();
	GLCD_Init();
	__disable_irq();

  GLCD_Clear(White);
  GLCD_DisplayString(0, 0, 1, "Hello World");
  // Now for interupts
  timerInit();
	
	// Set direction to output
  LPC_GPIO1->FIODIR |= 0xB0000000;
  LPC_GPIO2->FIODIR |= 0x0000007C;	
	
	__enable_irq();
	
	delay(5000);
	
  // Attempt to turn on LED 1 (port 1 pin 28)
  LPC_GPIO1->FIOSET = 1 << 28;
	
	delay(10000);
	
	// And Turn it back off agian
	LPC_GPIO1->FIOCLR = 1 << 28;
	
  GLCD_Clear(White);
	GLCD_DisplayString(0, 0, 1, "Finshed Delay Testing!");
	
	return 0;	
}
