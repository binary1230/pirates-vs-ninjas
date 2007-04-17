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
#define  ALLEGRO_STATICLINK
#endif // WIN32
#include <allegro.h>
#ifdef WIN32
#include <winalleg.h>
#endif // WIN32
#include <alleggl.h>

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

// Our rarely-modified engine stuff
#include "StdString.h"
#include "xmlParser.h"
#include "loadpng.h"
#include "alogg.h"

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
#include <unistd.h>
#include <fcntl.h>

// STL stuff
#include <list>
#include <map>
#include <vector>
#include <algorithm>

// 3d model support not ready yet.
// #include "Model_3DS.h"

#include "globals.h"
#include "singleton.h"

#endif // PRECOMPILED_H
