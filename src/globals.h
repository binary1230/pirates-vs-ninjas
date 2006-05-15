/* Global vars/defines */

#ifndef GLOBALS_H
#define GLOBALS_H

#include <config.h>
#include <assert.h>
#include <stdio.h>

typedef unsigned int uint;

// lameass .dat file functions
//#define E(x) ((BITMAP*)e[x].dat)
//#define S(x) ((BITMAP*)s[x].dat)
//#define SOUNDS(x) ((SAMPLE*)sounds[x].dat)

#define Rand(lower, upper) \
((rand() % (upper - lower + 1)) + lower)

#define DEFAULT_SCREEN_SIZE_X	320
#define DEFAULT_SCREEN_SIZE_Y 240

// PACKAGE and VERSION come from autotools (config.h)
#define VERSION_STRING PACKAGE"-"VERSION

// the fps (not ever changed)
// timestep is 1/FPS
#define FPS 30
#define TIMESTEP 1/FPS

#endif // GLOBALS_H
