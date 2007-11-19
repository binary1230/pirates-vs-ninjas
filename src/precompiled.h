// Precompiled header file.

// ONLY PUT SYSTEM-LEVEL INCLUDES HERE
// do NOT put anything that is in the game engine in here,
// with the exception of rarely-modified stuff

#ifndef PRECOMPILED_H
#define PRECOMPILED_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifndef WIN32
#define ENGINE_USES_CEGUI
#endif

#ifdef ENGINE_USES_CEGUI
#include <CEGUI.h>
#include <CEGUIWindowManager.h>
#include <RendererModules/OpenGLGUIRenderer/openglrenderer.h>
#include <CEGUIDefaultResourceProvider.h>
#endif // ENGINE_USES_CEGUI

// The order of the allegro stuff is SUPER-IMPORTANT
#ifdef WIN32
	#pragma warning(disable:4312) // 'type cast' : conversion from 'unsigned int' to 'unsigned char *' of greater size
	#pragma warning(disable:4267)
	#pragma warning(disable:4311) // pointer truncation from 'const void *' to 'unsigned long'

	// NOTE: Turning this off won't check for invalid iterators, HOWEVER, it is SLOW as hell.
	#ifndef _DEBUG
	#define _SECURE_SCL 0
	#define _HAS_ITERATOR_DEBUGGING 0
#endif // _DEBUG

#define  ALLEGRO_STATICLINK

#endif // WIN32

#include <allegro.h>

#ifdef WIN32
#include <winalleg.h>
#endif // WIN32

#include <alleggl.h>
#include <allegro/internal/aintern.h>

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

// Common to everything
#include <stdarg.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <png.h>
#include <GL/gl.h>

// Unix-y?
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifndef WIN32
#include <unistd.h>
#endif // WIN32

#include <fcntl.h>

// so that std::min and std::max work OK.
#undef min
#undef max 

// Our rarely-modified engine stuff
#include "StdString.h"
#define CString CStdStringA

// #include "Model_3DS.h" // not ready yet/ever
#include "xmlParser.h"
#include "loadpng.h"
#include "alogg.h"
#ifdef _MSC_VER
#include "XGetopt.h"	// include our own getopt() for native windows builds
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
