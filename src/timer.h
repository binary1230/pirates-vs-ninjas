//! See gameState.cpp for an explanation of this file's stuff

#ifndef TIMER_H
#define TIMER_H

//! Incremented by Timer()
extern int g_iOutstanding_updates;
extern int g_iTicks;

extern ALLEGRO_TIMER* g_timer;

//! A function which gets called once every 1/30th of a second
void OnTimer();

#endif // TIMER_H
