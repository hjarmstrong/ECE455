/*
    FreeRTOS V6.1.0 - Copyright (C) 2010 Real Time Engineers Ltd.

    ***************************************************************************
    *                                                                         *
    * If you are:                                                             *
    *                                                                         *
    *    + New to FreeRTOS,                                                   *
    *    + Wanting to learn FreeRTOS or multitasking in general quickly       *
    *    + Looking for basic training,                                        *
    *    + Wanting to improve your FreeRTOS skills and productivity           *
    *                                                                         *
    * then take a look at the FreeRTOS books - available as PDF or paperback  *
    *                                                                         *
    *        "Using the FreeRTOS Real Time Kernel - a Practical Guide"        *
    *                  http://www.FreeRTOS.org/Documentation                  *
    *                                                                         *
    * A pdf reference manual is also available.  Both are usually delivered   *
    * to your inbox within 20 minutes to two hours when purchased between 8am *
    * and 8pm GMT (although please allow up to 24 hours in case of            *
    * exceptional circumstances).  Thank you for your support!                *
    *                                                                         *
    ***************************************************************************

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation AND MODIFIED BY the FreeRTOS exception.
    ***NOTE*** The exception to the GPL is included to allow you to distribute
    a combined work that includes FreeRTOS without being obliged to provide the
    source code for proprietary components outside of the FreeRTOS kernel.
    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
    more details. You should have received a copy of the GNU General Public
    License and the FreeRTOS license exception along with FreeRTOS; if not it
    can be viewed here: http://www.freertos.org/a00114.html and also obtained
    by writing to Richard Barry, contact details for whom are available on the
    FreeRTOS WEB site.

    1 tab == 4 spaces!

    http://www.FreeRTOS.org - Documentation, latest information, license and
    contact details.

    http://www.SafeRTOS.com - A version that is certified for use in safety
    critical systems.

    http://www.OpenRTOS.com - Commercial support, development, porting,
    licensing and training services.
*/

/*
 * This is a very simple demo that demonstrates task and queue usages only in
 * a simple and minimal FreeRTOS configuration.  Details of other FreeRTOS 
 * features (API functions, tracing features, diagnostic hook functions, memory
 * management, etc.) can be found on the FreeRTOS web site 
 * (http://www.FreeRTOS.org) and in the FreeRTOS book.
 *
 * Details of this demo (what it does, how it should behave, etc.) can be found
 * in the accompanying PDF application note.
 *
*/

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "edf.h"

/* Standard include. */
#include <stdio.h>

/* Priorities at which the tasks are created. */
#define	TASK_PRIORITY		( tskIDLE_PRIORITY + 1 )

/* The rate at which data is sent to the queue, specified in milliseconds. */
#define mainQUEUE_SEND_FREQUENCY_MS			( 10 / portTICK_RATE_MS )

/* The number of items the queue can hold.  This is 1 as the receive task
will remove items as they are added, meaning the send task should always find
the queue empty. */
#define mainQUEUE_LENGTH					( 1 )

/* The ITM port is used to direct the printf() output to the serial window in 
the Keil simulator IDE. */
#define mainITM_Port8(n)    (*((volatile unsigned char *)(0xE0000000+4*n)))
#define mainITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))
#define mainDEMCR           (*((volatile unsigned long *)(0xE000EDFC)))
#define mainTRCENA          0x01000000

/*
 * Redirects the printf() output to the serial window in the Keil simulator
 * IDE.
 */
int fputc( int iChar, FILE *pxNotUsed );

/* One array position is used for each task created by this demo.  The 
variables in this array are set and cleared by the trace macros within
FreeRTOS, and displayed on the logic analyzer window within the Keil IDE -
the result of which being that the logic analyzer shows which task is
running when. */
unsigned long ulTaskNumber[ configEXPECTED_NO_RUNNING_TASKS ];

// The period and exicution time are const, and must be initilised now

/* Problem 1 */
//struct TaskControlBlock t1 = {.time = 1, .period = 4};
//struct TaskControlBlock t2 = {.time = 2, .period = 6};
//struct TaskControlBlock t3 = {.time = 3, .period = 8};

/* Problem 2 */
struct TaskControlBlock t1 = {.time = 1, .period = 4};
struct TaskControlBlock t2 = {.time = 2, .period = 6};
struct TaskControlBlock t3 = {.time = 5, .period = 12};

struct TaskControlBlock *TCB[3] = {&t1, &t2, &t3};

/*-----------------------------------------------------------*/

// Tasks

static void t1Code(void *args)
{
	while(1)
	{
		// Wait for 1000ms/1s, which is our time quantum.
		// I.E. no delines can change in 1 second so we are safe to "run"
	
		portTickType now = xTaskGetTickCount();
		while(xTaskGetTickCount() - now < 1000)
		{
		}
		// Invoke the Scheduler
		schedule();
	}
}

static void t2Code(void *args)
{
	while(1)
	{
		// Wait for 1000ms/1s, which is our time quantum.
		// I.E. no delines can change in 1 second so we are safe to "run"
	
		portTickType now = xTaskGetTickCount();
		while(xTaskGetTickCount() - now < 1000)
		{
		}
		// Invoke the Scheduler
		schedule();
	}
}

static void t3Code(void *args)
{
	while(1)
	{
		// Wait for 1000ms/1s, which is our time quantum.
		// I.E. no delines can change in 1 second so we are safe to "run"
	
		portTickType now = xTaskGetTickCount();
		while(xTaskGetTickCount() - now < 1000)
		{
		}
		// Invoke the Scheduler
		schedule();
	}
}

/*-----------------------------------------------------------*/

int main(void)
{
	/* Create Tasks */

	xTaskCreate(t1Code, (signed char *)"Task 1", configMINIMAL_STACK_SIZE, NULL, TASK_PRIORITY, &t1.handle);
	xTaskCreate(t2Code, (signed char *)"Task 2", configMINIMAL_STACK_SIZE, NULL, TASK_PRIORITY, &t2.handle);
	xTaskCreate(t3Code, (signed char *)"Task 3", configMINIMAL_STACK_SIZE, NULL, TASK_PRIORITY, &t3.handle);
	
	/* Call the EDF scheduler to Pick the initial Task to run */
	schedule();

	/* Start the tasks running. */
	vTaskStartScheduler();

	/* If all is well we will never reach here as the scheduler will now be
	running.  If we do reach here then it is likely that there was insufficient
	heap available for the idle task to be created. */
	for( ;; );
}

/*-----------------------------------------------------------*/

int fputc( int iChar, FILE *pxNotUsed ) 
{
	/* Just to avoid compiler warnings. */
	( void ) pxNotUsed;

	if( mainDEMCR & mainTRCENA ) 
	{
		while( mainITM_Port32( 0 ) == 0 );
		mainITM_Port8( 0 ) = iChar;
  	}

  	return( iChar );
}
