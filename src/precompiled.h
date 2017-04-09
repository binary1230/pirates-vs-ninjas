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

#define ENGINE_USES_SLOW_CHECKED_STL 0	// 2017, enabling this breaks, like, everything.


#ifdef _WIN64

	// yiiiiiiiiikes. should probbly fix everything here. 2017
	#pragma warning(disable:4312) // 'type cast' : conversion from 'unsigned int' to 'unsigned char *' of greater size
	#pragma warning(disable:4267)
	#pragma warning(disable:4311) // pointer truncation from 'const void *' to 'unsigned long'
	#pragma warning(disable:4996) // 'stricmp': The POSIX name for this item is deprecated.

	// NOTE: Turning this off won't check for invalid iterators, HOWEVER, it is SLOW as hell.
	#if ENGINE_USES_SLOW_CHECKED_STL
	#define _SECURE_SCL 0
	#define _HAS_ITERATOR_DEBUGGING 0    // 2017, enabling this breaks, like, everything.
	#endif // _DEBUG

	// #define  ALLEGRO_STATICLINK   // old? 2017
#endif // WIN32

// -----------------------------

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>

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

#define ENGINE_EMBEDDED_LUA			1
#define ENGINE_USES_BOX2D_PHYSICS	1
#define ENGINE_USES_BOOST			1

#if ENGINE_EMBEDDED_LUA
	#include <lua.hpp>
#endif

#if ENGINE_USES_BOX2D_PHYSICS
	#include <Box2D.h>
#endif 

#include <cstdio> // remove

#if ENGINE_USES_BOOST
	#include <boost/archive/tmpdir.hpp>
	#include <boost/archive/xml_iarchive.hpp>
	#include <boost/archive/xml_oarchive.hpp>
	#include <boost/archive/text_oarchive.hpp>
	#include <boost/archive/text_iarchive.hpp>

	#include <boost/serialization/export.hpp>

	#include <boost/serialization/vector.hpp>
	#include <boost/serialization/list.hpp>
	#include <boost/serialization/string.hpp>

	#include <boost/config.hpp>
	#if defined(BOOST_NO_STDC_NAMESPACE)
	namespace std {
		using ::remove;
	}
	#endif
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

// #include <atlstr.h>
typedef std::string CString;

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
#include <cctype>
#include <cstdlib>
#include <cstdarg>
#include <iomanip>
#include <iostream>
#include <fstream>

#include "globals.h"
#include "singleton.h"

#endif // PRECOMPILED_H
