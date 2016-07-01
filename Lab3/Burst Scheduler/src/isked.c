#include <lpc17xx.h>
#include <stdio.h>
#include "System/GLCD.h"
#include "System/LCDAssert.h"
#include "isked.h"
#include "timer.h"
#include "button.h"

#define MAX_BURST_SIZE 3

int enabeled = 1;
int count = 0;

void schedulerInit(unsigned int maxArivalRate)
{
	// Initially, the clock goes from 0 -> 1 and interupts, we want
	// it to start as 1 and go from 1 -> 0 -> 1 and then interupt/
	LPC_TIM1->TC = 1;
	
	// The TC ticks: 1 -> 0 -> 1, then interupts, so the prescale 
	// Timer must count half of one second, which at 25Mhz, is: 
	// (25 - 1) M /2 = 12499999 prescale ticks. because 1 tick
	// occurs when TC ticks.
	LPC_TIM1->PR = maxArivalRate;

	LPC_TIM1->MR0 = 1;
  
	// Set TC to 0 when it reaches 1, and interupt when TC reaches MR0
	LPC_TIM1->MCR = 1 | (1 << 1);

	NVIC_EnableIRQ(TIMER1_IRQn);

	// Finally, we enable the Timer
	LPC_TIM1->TCR = 1;
}

void TIMER1_IRQHandler(void) 
{
	// Acknowledge the Interupt
  LPC_TIM1->IR |= 1;
	
	count = 0;
	
	GLCD_DisplayString(0, 0, 1, "New Interval!           ");
	
	if(enabeled)
	{
		return;
	}
	
	enabeled = 1;
	
	// clear interupts that occured while we waited.
	LPC_GPIOINT->IO2IntClr |= (1<<10);
	NVIC_ClearPendingIRQ(EINT3_IRQn);
	
	// And turn the Interupt back on
	NVIC_EnableIRQ(EINT3_IRQn);
}

void notifyScheduler(void)
{
	count++;
	
	unsigned char s[] = "X Interupts Receieved!";
	s[0] = count + '0';
	GLCD_DisplayString(0, 0, 1, s);
	
	if(count < MAX_BURST_SIZE)
	{
		return;
	}
	
	GLCD_DisplayString(0, 0, 1, "Burst Size Reached!     ");
	NVIC_DisableIRQ(EINT3_IRQn);
	enabeled = 0;
}
