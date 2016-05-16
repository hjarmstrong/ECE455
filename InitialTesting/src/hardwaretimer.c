#include <lpc17xx.h>

static long long int g_timer_count;

void delay(int ms)
{ 
	__disable_irq();
	long long int stop = g_timer_count + ms;
	__enable_irq();
	
  while(g_timer_count < stop)
	{
		// wait for it :)
	}
}


void TIMER0_IRQHandler(void) 
{
	g_timer_count++;
	/* ack interrupt, see section  21.6.1 on pg 493 of LPC17XX_UM */
  LPC_TIM0->IR = 1;
}


void timerInit(void)
{
    g_timer_count = 0;

	/**
     * Step 4.1: Prescale Register PR setting 
     * CCLK = 100 MHZ, PCLK = CCLK/4 = 25 MHZ
     * 2*(12499 + 1)*(1/25) * 10^(-6) s = 10^(-3) s = 1 ms
     * TC (Timer Counter) toggles b/w 0 and 1 every 12500 PCLKs
     * see MR setting below 
     */
    LPC_TIM0->PR = 24999; // prob should be 12499, but it appears my PLL is running at 800 Mhz for some reason => CCL is 200 => PCL is 50
	// I'm comming arround on this number... if our frequency is 25 million cycles per second, then we should hit one second in 25M increments, or one ms in 25M/1000 = 25000 increments.....
	// So was Irene's old sysem every half MS... where does the 10^-6 come in?????

    /** Step 4.2: MR setting, see section 21.6.7 on pg496 of LPC17xx_UM. */
    LPC_TIM0->MR0 = 1;

    /**
     * Step 4.3: MCR setting, see table 429 on pg496 of LPC17xx_UM.
     * Interrupt on MR0: when MR0 mathches the value in the TC, 
     * generate an interrupt.
     * Reset on MR0: Reset TC if MR0 mathches it.
     */
    LPC_TIM0->MCR = 1 | (1 << 1);

	/** Step 4.4: CSMSIS enable timer0 IRQ */
    NVIC_EnableIRQ(TIMER0_IRQn);

    /** Step 4.5: Enable the TCR. See table 427 on pg494 of LPC17xx_UM. */
    LPC_TIM0->TCR = 1;
}
