#include "states.h" 

// Global Varibles
struct state morse;
struct state input;

void setTransition(struct state *me, INPUT_TYPE *val, STATE_SIZE_T from, STATE_SIZE_T to)
{
	for(int i = 0; ; i++)
	{
		if(val[i] == 0) // Transition Strings are null terminated
			return;
    me->transition[from][val[i]] = to;
	}
}

void createFSM(struct state *me, STATE_SIZE_T numStates, STATE_SIZE_T initialState)
{
	me->numStates = numStates;
	me->initialState = initialState;
	me->currState = initialState;
	
	INPUT_TYPE transString[2];
	transString[1] = 0; // Null terminated transition String
	
	for(STATE_SIZE_T i = 0; i < numStates; i++)
  {
		for(INPUT_TYPE j = 1; j < INPUT_SIZE; j++)
		{
			transString[0] = j;
			setTransition(me, transString, i, 0); // Initilize all states to NULL
			// me.transition[i][j] = 0;
		}
  }	
}

void resetFSM(struct state *me)
{
	me->currState = me->initialState;
}

STATE_SIZE_T transition(struct state *me, INPUT_TYPE val)
{
	return (me->currState = me->transition[me->currState][val]);
}

int isNull(struct state *me)
{
	if(me->currState == 0)
		return 1;
	
	return 0;
}
