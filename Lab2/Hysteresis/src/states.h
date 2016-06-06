#ifndef STATES_H
#define STATES_H

#include <limits.h>

#define MAX_STATES 20
#define INPUT_SIZE UCHAR_MAX
#define STATE_SIZE_T unsigned char
#define INPUT_TYPE char

struct state
{
	STATE_SIZE_T numStates;
	STATE_SIZE_T initialState;
	STATE_SIZE_T transition[MAX_STATES][INPUT_SIZE];
	STATE_SIZE_T currState;
};

void createFSM(struct state *me, STATE_SIZE_T numStates, STATE_SIZE_T initialState);
void resetFSM(struct state *me);
void setTransition(struct state *me, INPUT_TYPE *val, STATE_SIZE_T from, STATE_SIZE_T to);
STATE_SIZE_T transition(struct state *me, INPUT_TYPE val); // Returns state switched to... NULL == BAD
int isNull(struct state *me);

extern struct state input;
extern struct state furnace;

enum Input
{
	Input_NUL,
	Input_Start,
	Debounce,
	InProgress,
	IsDash,
	DashDebounce,
	DotDebounce,
	NUM_INPUT_STATES
};

enum Furnace
{
	Furnace_NUL,
	FurnaceOn,
	FurnaceOff,
	NUM_FURNACE_STATES
};


#endif
