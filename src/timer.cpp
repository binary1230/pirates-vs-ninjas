#include "stdafx.h"
#include "timer.h"
#include "globals.h"

// For timing stuff
volatile int g_iOutstanding_updates = 0;
volatile int g_iTicks = 0;

void Timer(void) {
	g_iOutstanding_updates++;
	g_iTicks++;
} END_OF_FUNCTION(Timer);
