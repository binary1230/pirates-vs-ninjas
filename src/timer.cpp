#include "stdafx.h"
#include "timer.h"
#include "globals.h"

// TODO: move this stuff to gameState.h/.cpp, doesn't need to be separate anymore.

// For timing stuff
int g_iOutstanding_updates = 0;
int g_iTicks = 0;

ALLEGRO_TIMER* g_timer;

void OnTimer(void) {
	g_iOutstanding_updates++;
	g_iTicks++;
}
