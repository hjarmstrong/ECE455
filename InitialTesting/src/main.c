#include <lpc17xx.h>
#include "System/GLCD.h"
#include "timer.h"

void displayTime(unsigned char sec, int min, char *s)
{
	if(sec > 60)
	{
		GLCD_DisplayString(0, 0, 1, "Bad Seconds");
		while(1) {}
	}
	
	int i = 6;
	int si = 0;
	int digits[8] = {0};
	
	while(min)
	{
		digits[i--] = min % 10;
		min /= 10;
	}
	
	for(i++; i < 7; i++)
	{
		  s[si++] = '0' + digits[i];
	}
	
	// Show 0: before 1 minute
	if(si == 0)
	{
		s[si++] = '0';
	}
	
	s[si++] = ':';
	for(int j = 0; j < 7; j++)
	{
		digits[j] = 0;
	}
	
	i = 6;
	
	while(sec)
	{
		digits[i--] = sec % 10;
		sec /= 10;
	}
	
	int oldsi = si;
	
	for(i++; i < 7; i++)
	{
		  s[si++] = '0' + digits[i];
	}
	
	if(si == oldsi)
	{
		s[si++] = '0';
		// s[si++] = '0'; would somehow also need two diigits when sec == 1, 2, 3, ... , 9
	}
	
	s[si] = '\0';
	GLCD_DisplayString(0, 0, 1, (unsigned char *)s);
}

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
	
	delay(10000);
		
	unsigned char sec = 0;
	
	int min = 0;
	
	char time[] = "XXXXXXX:XX";
	
	// This will overflow after ~69000 days
	while(1)
	{
		delay(1000);
		if(++sec == 60)
		{
			min++;
			sec = 0;
		}
		GLCD_Clear(White);
		displayTime(sec, min, time);
	}
	
}
