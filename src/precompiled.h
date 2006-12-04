// Precompiled header file.

// ONLY PUT SYSTEM-LEVEL INCLUDES HERE
// do NOT put anything that is in the game engine in here,
// with the exception of rarely-modified stuff

#ifndef PRECOMPILED_H
#define PRECOMPILED_H

// The order of these two is SUPER-IMPORTANT
#include <allegro.h>
#ifdef WIN32
#include <winalleg.h>
#endif
#include <alleggl.h>

#ifdef PLATFORM_DARWIN
#include <CoreServices/CoreServices.h>
#include <sys/malloc.h>
#else
#include <malloc.h>
#endif // PLATFORM_DARWIN


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
#include <algorithm>
#include <list>
#include <map>
#include <vector>

using namespace std;

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

// Our rarely-modified engine stuff
#include "StdString.h"
#include "xmlParser.h"
#include "alogg.h"
#include "loadpng.h"
#include "Model_3DS.h"

#endif // PRECOMPILED_H
