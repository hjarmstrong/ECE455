#include <lpc17xx.h>
#include <stdio.h>
#include "System/GLCD.h"
#include "System/LCDAssert.h"
#include "isked.h"
#include "timer.h"
#include "button.h"

// 5 seconds as number of 25MHz clock cycles
#define INTER_ARIVAL_TIME 124999999

int enabeled = 1;

void notifyScheduler(void)
{
	GLCD_DisplayString(0, 0, 1, "InteruptsOFF");
	NVIC_DisableIRQ(EINT3_IRQn);
	enabeled = 0;
	timerInit(INTER_ARIVAL_TIME);
}

void timerExpired(void)
{
	if(enabeled)
	{
		LCD_ASSERT(0);
	}
	
	GLCD_DisplayString(0, 0, 1, "Interupts On");
	enabeled = 1;
	
	// clear interupts that occured while we waited.
	LPC_GPIOINT->IO2IntClr |= (1<<10);
	NVIC_ClearPendingIRQ(EINT3_IRQn);
	
	// And turn the Interupt back on
	NVIC_EnableIRQ(EINT3_IRQn);
}
