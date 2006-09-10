// stdafx.h - global header file for all .cpp files
// however, please add any extra #include"" 's to 
// precompiled.h which is the real precompiled header file

#ifndef STDAFX_H
#define STDAFX_H

// only place we include config.h in the project
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

// a bit of a hack to implement precompiled headers
#ifndef PRECOMPILE_ENABLE
#include "precompiled.h"
#endif

#endif // STDAFX_H
