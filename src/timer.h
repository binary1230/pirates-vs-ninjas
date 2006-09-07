//! See gameState.cpp for an explanation of this file's stuff

#ifndef TIMER_H
#define TIMER_H

#include "stdafx.h"

//! Incremented by Timer()
extern volatile int outstanding_updates;

//! A function which gets called once every 1/30th of a second
void Timer();

#endif // TIMER_H
