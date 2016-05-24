#include <lpc17xx.h>
#include "System/GLCD.h"
#include "states.h" 

int main(void)
{
  SystemInit();
	GLCD_Init();
	
	GLCD_Clear(White);
	GLCD_DisplayString(0, 0, 1, "Hello World");
	while(1) {}
}
