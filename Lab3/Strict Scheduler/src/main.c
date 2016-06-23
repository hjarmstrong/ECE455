#include <lpc17xx.h>
#include "System/LCDAssert.h"
#include "System/GLCD.h"

int main(void)
{
  SystemInit();
	GLCD_Init();
	
	GLCD_Clear(White);

	
	while(1)
	{
		LCD_ASSERT(0);
	}
}
