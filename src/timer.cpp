#include "timer.h"

// For timing stuff
volatile int outstanding_updates = 0;

void Timer(void) {
	outstanding_updates++;
} END_OF_FUNCTION(Timer);
