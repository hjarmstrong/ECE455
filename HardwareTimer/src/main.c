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
	
	if((i + 1) == (7-1))
	{
     s[si++] = '0';
	}
	
	for(i++; i < 7; i++)
	{
    s[si++] = '0' + digits[i];
	}

	if(si == oldsi)
	{
		s[si++] = '0';
		s[si++] = '0'; 
	}
	
	s[si] = '\0';
	GLCD_DisplayString(0, 0, 1, (unsigned char *)s);
}

int main(void)
{
  SystemInit();
	__disable_irq();
	GLCD_Init();
  timerInit();	
	__enable_irq();
			
	char time[] = "XXXXXXX:XX";
	
	// This will overflow after ~69000 days
	while(1)
	{
		//GLCD_Clear(White);
		displayTime(g_timer_seconds, g_timer_minutes, time);
	}
	
}
