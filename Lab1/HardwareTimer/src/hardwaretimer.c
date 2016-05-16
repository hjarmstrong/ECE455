#include <lpc17xx.h>

int g_timer_seconds;
int g_timer_minutes;

void TIMER0_IRQHandler(void) 
{	
	/* ack interrupt, see section  21.6.1 on pg 493 of LPC17XX_UM */
  LPC_TIM0->IR |= 1;
	
	if(++g_timer_seconds == 60)
	{
		g_timer_minutes++;
		g_timer_seconds = 0;
	}
}


void timerInit(void)
{
    g_timer_seconds = 0;
	  
	  g_timer_minutes = 0;
	
	  LPC_TIM0->TC = 1;
	
	  // The TC ticks: 1 -> 0 -> 1, then interupts, so the prescale 
	  // Timer must count half of one second, which at 25Mhz, is: 
	  // 25 M /2 = 12499999 ticks.
    LPC_TIM0->PR = 12499999;

    LPC_TIM0->MR0 = 1;

    LPC_TIM0->MCR = 1 | (1 << 1);

    NVIC_EnableIRQ(TIMER0_IRQn);

    LPC_TIM0->TCR = 1;
}
