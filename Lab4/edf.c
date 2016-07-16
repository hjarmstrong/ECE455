#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "edf.h"

void schedule(void)
{
	static unsigned long long time = 0;
	static int init = 0;

	unsigned int earlyDeadline = ~0u;
	int nextTask = -1; 
	
	retry:
	for(int i = 0; i < sizeof(TCB) / sizeof(TCB[0]); i++)
	{
		// New Tasks Arrive!
		if((time % TCB[i]->period) == 0)
		{
			if(TCB[i]->timeRemaining != 0) // Don't miss deadlines!
			{
				printf("Deadline Missed: There exists no valid schedule!\r\n");
				while(1) {}
			}
			TCB[i]->timeRemaining = TCB[i]->time;
			TCB[i]->deadline = time + TCB[i]->period;
		}
		
		// Now pick the earliest Deadline

		if((TCB[i]->deadline < earlyDeadline) 
			&& (TCB[i]->timeRemaining != 0))
		{
			earlyDeadline = TCB[i]->deadline;
			nextTask = i;
		}

	}
	
	if(nextTask == -1)
	{
		// Nothing to do but wait...
		portTickType lastWakeTime = xTaskGetTickCount();
		vTaskDelayUntil( &lastWakeTime, 1000 );
		time++;
		
		goto retry;
	}
	
	// Keep track of the quantum!
	time++;
	TCB[nextTask]->timeRemaining--;
		
	if(TCB[nextTask]->handle == xTaskGetCurrentTaskHandle())
	{
		return;
	}
	
	// The very first task is selected by main, not another task
	// So, we must make sure only that task runs when the FREERTOS
	// Scheduler runs. So, we suspend all user tasks.
	if(!init)
	{
		init = 1;
		for(int i = 0; i < sizeof(TCB) / sizeof(TCB[0]); i++)
		{
			if(TCB[nextTask]->handle != TCB[i]->handle)
			{
				vTaskSuspend(TCB[i]->handle);
			}
		}
		return;	
	}
	
	vTaskResume(TCB[nextTask]->handle);
	vTaskSuspend(NULL);
	return;
}
