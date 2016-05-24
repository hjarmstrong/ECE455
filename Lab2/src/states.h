#ifndef STATES_H
#define STATES_H

#include <limits.h>

#define MAX_STATES 20
#define INPUT_SIZE UCHAR_MAX


struct state createFSM(unsigned char numStates, unsigned char initialState);
void resetFSM(struct state);
void setTransition(unsigned char val, struct state from, struct state to);

struct state
{
	unsigned char numStates;
	unsigned char initialState;
	unsigned char transition[MAX_STATES][INPUT_SIZE];
};

#endif
