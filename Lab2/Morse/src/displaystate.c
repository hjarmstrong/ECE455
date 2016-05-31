#include <lpc17xx.h>
#include "System/GLCD.h"
#include "displaystate.h"

void DisplayState(unsigned char *s)
{
	static unsigned int line = INITIAL_LINE;
	
	GLCD_DisplayString((line - 1) % NUM_LINES, 0, 1, "   ");
	GLCD_DisplayString(line % NUM_LINES, 0, 1, " > ");
	GLCD_DisplayString(line % NUM_LINES, 3, 1, s);
	
	// In the case of overflow the integer will continue from zero
	// creating a slight jump back to the start every ~4 billion inputs
	line++;
}
