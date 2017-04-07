// Precompiled header file.

// ONLY PUT SYSTEM-LEVEL INCLUDES HERE
// do NOT put anything that is in the game engine in here,
// with the exception of rarely-modified stuff

#ifndef PRECOMPILED_H
#define PRECOMPILED_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

// -----------------------------
// Config options
// -----------------------------

// #define ENGINE_USES_CEGUI // not going to use anymore
// #define ENGINE_USES_FLTK // more ideas we're not going to use anymore :)

// #ifndef WIN32
// #undef ENGINE_USES_CEGUI
// #endif

#define ENGINE_USES_SLOW_CHECKED_STL 0

// The order of the allegro stuff is SUPER-IMPORTANT
#ifdef _WIN64
	#pragma warning(disable:4312) // 'type cast' : conversion from 'unsigned int' to 'unsigned char *' of greater size
	#pragma warning(disable:4267)
	#pragma warning(disable:4311) // pointer truncation from 'const void *' to 'unsigned long'
	#pragma warning(disable:4996) // 'stricmp': The POSIX name for this item is deprecated.

	// NOTE: Turning this off won't check for invalid iterators, HOWEVER, it is SLOW as hell.
	#ifdef ENGINE_USES_SLOW_CHECKED_STL
	#define _SECURE_SCL 0
	#define _HAS_ITERATOR_DEBUGGING 0
	#endif // _DEBUG

	// #define  ALLEGRO_STATICLINK
#endif // WIN32

// -----------------------------

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#ifdef _WIN64
#define _WIN32_WINNT   0x0400
#include <objbase.h>
#endif // WIN32

#include <allegro5/allegro_opengl.h>

#ifdef PLATFORM_DARWIN
#include <CoreServices/CoreServices.h>
#include <sys/malloc.h>
#else
#include <malloc.h>
#endif // PLATFORM_DARWIN

#define ENGINE_EMBEDDED_LUA

#ifdef ENGINE_EMBEDDED_LUA
extern "C" {
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}
#endif

#define ENGINE_USES_BOX2D_PHYSICS

#ifdef ENGINE_USES_BOX2D_PHYSICS
	#include <Box2D.h>
#endif 

// Common to everything
#include <stdarg.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <png.h>
// #include <GL/gl.h>

// Unix-y?
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifndef _WIN64
#include <unistd.h>
#endif // _WIN64

#include <fcntl.h>

// so that std::min and std::max work OK.
#undef min
#undef max 

// TODO: OLD, KILL?
// Our rarely-modified engine stuff
#include <atlstr.h>

#include "external/xmlParser.h"
#ifdef _MSC_VER
#include "external/XGetopt.h"	// include our own getopt() for native windows builds
#endif

// STL stuff
#include <list>
#include <map>
#include <vector>
#include <algorithm>
#include <functional>
#include <cstdarg>
#include <cstdio>
#include <cctype>
#include <cstdlib>
#include <cstdarg>

#include "globals.h"
#include "singleton.h"

#endif // PRECOMPILED_H
