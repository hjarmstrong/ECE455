struct TaskControlBlock
{
	const int time;
	const int period;
	
	int timeRemaining;
	int deadline;
	xTaskHandle handle;
};

extern struct TaskControlBlock t1;
extern struct TaskControlBlock t2;
extern struct TaskControlBlock t3;

extern struct TaskControlBlock *TCB[3];

void schedule(void);
