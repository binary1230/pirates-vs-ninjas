#ifndef allegro_wrapper_h
#define allegro_wrapper_h

#ifdef WIN32
#pragma warning (disable : 4312) // conversion from 'unsigned int' to 'unsigned char *' of greater size
#endif

// Pirates vs ninjas ONLY
// SUPER-IMPORTANT: These 2 #includes must come FIRST and be in this order
#include <allegro.h>
#ifdef WIN32
#include <winalleg.h>
#endif

#endif // allegro_wrapper_h