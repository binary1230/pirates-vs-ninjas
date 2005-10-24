/* Global vars/defines */

#ifndef GLOBALS_H
#define GLOBALS_H

// include master config.h from ./configure script
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

#define SCREEN_SIZE_X	640
#define SCREEN_SIZE_Y 480

// full version string 
#define VERSION_STRING PACKAGE"-"VERSION

#endif // GLOBALS_H
