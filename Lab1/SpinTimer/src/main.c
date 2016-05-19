#include <lpc17xx.h>
#include "System/GLCD.h"
#include "timer.h"

#define MAX_BUFFER_DIGITS 7

/*
   A function to display the time in the 
	 format, minutes:seconds, which requires
	 a buffer large enough to store the output
	 string to be passed in.
*/
void displayTime(unsigned char sec, int min, char *s)
{
	if(sec > 60)
	{
		GLCD_DisplayString(0, 0, 1, "Invalid Input");
		while(1) {}
	}
	
	// Digits are retrieved in reverse order, i tracks the floor
	// of the used digits. Si is the string buffers index, tracking
	// the location within the buffer.
	
	int i = MAX_BUFFER_DIGITS - 1;
	int si = 0;
	int digits[8] = {0};
	
	while(min)
	{
		digits[i--] = min % 10;
		min /= 10;
	}
	
	for(i++; i < MAX_BUFFER_DIGITS; i++)
	{
		  s[si++] = '0' + digits[i];
	}
	
	// Show 0: before 1 minute
	if(si == 0)
	{
		s[si++] = '0';
	}
	
	s[si++] = ':';
	for(int j = 0; j < MAX_BUFFER_DIGITS; j++)
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
	
	if((i + 1) == (MAX_BUFFER_DIGITS-1))
	{
     s[si++] = '0';
	}
	
	for(i++; i < MAX_BUFFER_DIGITS; i++)
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
	__enable_irq();
	
	GLCD_Clear(White);
			
	unsigned char sec = 0;
	
	int min = 0;
	
	char time[] = "XXXXXXX:XX";
	
	// The time will exceed the buffer after approximately 6000 days
	while(1)
	{
		displayTime(sec, min, time);
		delay(1000);
		if(++sec == 60)
		{
			min++;
			sec = 0;
		}
	}
}
