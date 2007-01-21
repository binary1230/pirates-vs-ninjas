/* Global vars/defines */

#ifndef GLOBALS_H
#define GLOBALS_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

typedef unsigned int uint;

// OLD UNUSUED [stupid] .dat file functions
//#define E(x) ((BITMAP*)e[x].dat)
//#define S(x) ((BITMAP*)s[x].dat)
//#define SOUNDS(x) ((SAMPLE*)sounds[x].dat)

#define Rand(lower, upper) \
((rand() % (int(upper) - int(lower) + 1)) + int(lower))

#define DEFAULT_SCREEN_SIZE_X	640
#define DEFAULT_SCREEN_SIZE_Y 480

// PACKAGE and VERSION come from autotools (config.h)
#ifdef HAVE_CONFIG_H
#define VERSION_STRING PACKAGE"-"VERSION
#else
#define VERSION_STRING "ninjas-engine"
#endif // HAVE_CONFIG_H

// the fps (not ever changed)
// timestep is 1/FPS
// #define FPS 60
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

// Misc: Take a string and split it based on a delimiter, put the
// resulting strings into a list.
// 
// For example: StringSplit("Hey|what's|up","|", out_list) will
// put 3 things, "Hey", "what's", and "Up", into out_list.
void StringSplit(CString str, CString delim, vector<CString> &results);

// TxtObject delimiters (see objectTxtOverlay.cpp)
#define OBJECT_TXT_PAGE_DELIM "~"			// Which char goes to the next page
#define OBJECT_TXT_LINE_DELIM "|"			// Which char goes to the next line

// DEBUG HACK NASTY STUFF
class Object;
#define LogObjectEvent(Event) LogEvent(Event, this)
enum LOGTYPE {
	OBJECT_NEW,
	OBJECT_INIT,
	OBJECT_SHUTDOWN,
	OBJECT_DELETE,
	OBJECT_LAYERADD,
	OBJECT_LAYERREMOVE
};
void LogEvent(LOGTYPE event, Object* obj);

#ifdef DEBUG_HACK_NASTY_STUFF
void InitDebugHackLog();
void ShutdownDebugHackLog();
#endif // DEBUG_HACK_NASTY_STUFF

#define SAFE_DELETE(x) \
				if (x) delete x; x = NULL

#endif // GLOBALS_H
