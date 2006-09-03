/* Global vars/defines */

#ifndef GLOBALS_H
#define GLOBALS_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

//! Set to '1' to run AI training functions
#define AI_TRAINING 1

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
#ifdef HAVE_CONFIG_H
#define VERSION_STRING PACKAGE"-"VERSION
#else
#define VERSION_STRING "ninjas-engine"
#endif // HAVE_CONFIG_H

// the fps (not ever changed)
// timestep is 1/FPS
#define FPS 30
#define TIMESTEP 1/FPS

//! A fudge value.  You can't compare two floats
//! directly, we just want to know if they are close.
//! This defines how close they must be to be considered
//! 'equal'
#define TOLERANCE 0.00001

//! The default debug message level.  0 = none, 1 = all
//! This value is overridden by the '-m' command line switch
#define DEFAULT_DEBUG_MSG_LEVEL 0

// (65536) size of audiostream buffer for music
#define DEFAULT_MUSIC_BUFFER_SIZE   (1<<16) 

// (32768) amount of data to read from disk each time 
#define DEFAULT_MUSIC_DATA_SIZE  (1<<15) 

#endif // GLOBALS_H
