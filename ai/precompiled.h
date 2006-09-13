// Precompiled header file.

// ONLY PUT SYSTEM-LEVEL INCLUDES HERE
// do NOT put anything that is in the game engine in here,
// with the exception of rarely-modified stuff

#ifndef PRECOMPILED_H
#define PRECOMPILED_H

// Common to everything
#include <assert.h>
#include <malloc.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <png.h>

// Unix-y?
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>

// STL stuff
#include <algorithm>
#include <list>
#include <map>
#include <vector>

using namespace std;

#include "StdString.h"

#endif // PRECOMPILED_H
