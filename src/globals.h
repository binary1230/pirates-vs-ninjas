/* Global vars/defines */

#ifndef GLOBALS_H
#define GLOBALS_H

#define ENGINE_VERSION "v0.1-alpha1"

#include "auto-config.h"

typedef unsigned int uint;

// lameass .dat file functions
//#define E(x) ((BITMAP*)e[x].dat)
//#define S(x) ((BITMAP*)s[x].dat)
//#define SOUNDS(x) ((SAMPLE*)sounds[x].dat)

#define Rand(lower, upper) \
((rand() % (upper - lower + 1)) + lower)

#define SCREEN_SIZE_X	640
#define SCREEN_SIZE_Y 480

#endif // GLOBALS_H
