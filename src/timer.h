#ifndef TIMER_H
#define TIMER_H

#include <allegro.h>

#include "globals.h"

extern volatile int outstanding_updates;
void Timer();

#endif // TIMER_H
