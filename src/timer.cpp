#include "timer.h"
#include "globals.h"

// For timing stuff
volatile int outstanding_updates = 0;
volatile int ticks = 0;

void Timer(void) {
	outstanding_updates++;
	ticks++;
} END_OF_FUNCTION(Timer);
