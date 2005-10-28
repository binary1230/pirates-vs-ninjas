// Deals with nasty platform specific stuff.  
// Tons of #ifdef fun

#ifndef PLATFORM_H
#define PLATFORM_H

#include <string.h>
#include <stdio.h>

#include "globals.h"

//! XXX STUPID HACKS FOR NOW
char* get_correct_path(const char* filename);
const char* get_current_working_dir();

#endif
