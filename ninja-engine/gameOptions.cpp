/* GameOptions.cpp
 * (c) 2017 Dominic Cerquetti, licensed under the GPL v2
 *
 * Handles command line runtime switches (like '-h' '-v' etc)
 *
 * ---------------------------------------------------------
 * This file contains code taken from OpenDarwin Project:
 * ---------------------------------------------------------
 *  Create and destroy argument vectors (argv's)
 *  Copyright (C) 1992, 2001 Free Software Foundation, Inc.
 *  Written by Fred Fish @ Cygnus Support
 *
 *  This file contains part of the libiberty library. (GPL)*/

#include "stdafx.h"
#include "gameOptions.h"
#include "basename.h"
#include "window.h"
#include "globals.h"
#include "assetManager.h"

DECLARE_SINGLETON(GameOptions)

void GameOptions::PrintBanner() {
	TRACE(
	"Ninja Engine (%s)\n"
	"binary1230(at)yahoo.com | http://github.com/binary1230/ninjas-engine\n"
	"(c) 2017 Dominic Cerquetti, this program is Free Software\n"
	"Licensed under the GNU GPL v2, see http://gnu.org\n\n",
	VERSION_STRING);

	TRACE("Current working directory: %s", AssetManager::GetCurrentWorkingDir().c_str());
	TRACE("Current EXE Path: %s", AssetManager::GetCurrentExeFullPath().c_str());
}

void GameOptions::PrintOptions(const char* arg0) {
	arg0 = basename(arg0);
	if (!show_help) {
		TRACE("type '%s -h' for more options..\n\n", arg0);
	} else {
		TRACE(
		"NOTE: data files MUST be in a dir in THIS folder called 'data'\n\n"

		"Usage: %s [options]\n"
		"-f            | try to force fullscreen mode\n"
		"-w            | try to force windowed mode\n"
		"-g mode       | 0 = no buffering, 1 = double buffer [default]\n"
		"              | 2 = page flipping, 3 = triple buffering\n\n"
		
		"-m xmlfile    | specify a different mode to use on startup\n\n"
		
		"-r file       | record a demo to 'file'\n"
		"-d file       | playback a demo from 'file'\n\n"

		"-X            | disable sound\n\n"

		"-a            | (EXPERIMENTAL) start as animation editor\n\n"

		"-2            | (DEBUG) use 640x480 instead of 320x240\n"
		"-z            | (DEBUG) use 800x600 (z?? z?? wtf.)\n"
		"-p            | (DEBUG) start the game paused (press F1 and F2 in game)\n"
		"-v            | (DEBUG) show debugging messages\n\n"

		"-8            | Run game as fast as possible (not for human playing)\n"
		"-9            | Do not do anything interactive (not for human playing)\n"

		"-h            | display this help message\n\n"

		, arg0);
	}
}

void GameOptions::Clear() {
	fullscreen = false;
	show_help = false;
	_MapEditorEnabled = false;
	
	_FirstMode = "";

	record_demo = false;
	playback_demo = false;
	
	demo_filename = "";
	
	sound_enabled = true;
	
	debug_start_paused = false;
	debug_message_level = DEFAULT_DEBUG_MSG_LEVEL;

	draw_graphics = true;
	wait_for_updates = true;

	is_valid = true;
}

/* @deftypefn Extension void freeargv (char **@var{vector})

Free an argument vector that was built using @code{buildargv}.  Simply
scans through @var{vector}, freeing the memory for each argument until
the terminating @code{NULL} is found, and then frees @var{vector}
itself.

@end deftypefn */
void freeargv (char** vector) {
  register char **scan;

  if (vector != NULL) {
		for (scan = vector; *scan != NULL; scan++) {
			free (*scan);
		}
    
		free (vector);
	}
}

/* @deftypefn Extension char** dupargv (char **@var{vector})

Duplicate an argument vector.  Simply scans through @var{vector},
duplicating each argument until the terminating @code{NULL} is found.
Returns a pointer to the argument vector if successful.  Returns
@code{NULL} if there is insufficient memory to complete building the
argument vector.

@end deftypefn */
char ** dupargv (const char **argv) {
  int argc;
  char **copy;
  
  if (argv == NULL)
    return NULL;
  
  /* the vector */
  for (argc = 0; argv[argc] != NULL; argc++);
  copy = (char **) malloc ((argc + 1) * sizeof (char *));
  if (copy == NULL)
    return NULL;
  
  /* the strings */
  for (argc = 0; argv[argc] != NULL; argc++) {
    int len = strlen (argv[argc]);
    copy[argc] = (char*)malloc (sizeof (char *) * (len + 1));
    if (copy[argc] == NULL)	{
		 	freeargv (copy);
		  return NULL;
		}
  	strcpy (copy[argc], argv[argc]);
  }
  copy[argc] = NULL;
  return copy;
}

bool GameOptions::ParseArguments(const int argc, const char* argv[]) 
{	
	PrintBanner();

	Clear();

	if (!argv) {
		return true;
	}

	char** new_argv = dupargv(argv);
	
	if (!new_argv) {
		return (is_valid = false);
	}

	bool _fullscreen_option_set = false;
	char c;

	while ( (c = getopt(argc,new_argv,"fzwm:r:d:X23vsc:p:h89ea")) != -1) {
		switch (c) {

			case 'm':
				_FirstMode = optarg;
				break;

			case 'a':
				_FirstMode = "data/animationeditor.xml"; // such a dumb hacky way to start
				break;

			// get demo filename
			case 'r': case 'd':
				if (demo_filename.length() > 0) {
						TRACE(	"Options ==> ERROR "
														"Don't give more than 1 demo filename (-r, -d)\n");
						return (is_valid = false);
				}

				if (c == 'r')
						record_demo = true;
				else 
						playback_demo = true;
								
				demo_filename = std::string(optarg);
				break;
			
			// display help
			case 'h':
				show_help = true;
				return (is_valid = false);
				break;

			// disable sound
			case 'X':
				sound_enabled = false;
				break;
	
			// fullscreen or windowed
			case 'f': case 'w':
				if (_fullscreen_option_set) {
					TRACE("Options ==> ERROR, Cannot select both fullscreen (-f) and windowed (-w) mode.\n");
					return (is_valid = false);
				} else if (c == 'f') {
					fullscreen = true;
					_fullscreen_option_set = true;
					TRACE("Options ==> fullscreen mode enabled\n");
				} else if (c == 'w') {
					fullscreen = false;
					_fullscreen_option_set = true;
					TRACE("Options ==> windowed mode enabled\n");
				}
				break;


			// debug: hack - '2x size'
			case '2':
				screen_size_x = 640;
				screen_size_y = 480;
				break;
			
			// debug: hack - '2x size'
			// ZZZZ?? WHY Z? WHAT THE SHITTING BITCH CAT, DOM.
			case 'z':
				screen_size_x = 800;
				screen_size_y = 600;
				break;

			// debug: start in 'paused' mode, press F1 to go, F2 to step
			case '3':
				debug_start_paused = true;
				break;

			// debug: enable extra debug messages
			case 'v':
				debug_message_level = 1;
				break;
	
			// Update as fast as possible, useful for AI training
			case '8':
				wait_for_updates = false;
				break;

			// Do not do anything interactive (for server or AI training)
			case '9':
				draw_graphics = false;
				sound_enabled = false;
				break;

			// ':' and '?' mean unrecognized
			default:
			case ':': case '?':
				TRACE("Unrecognized command line option '-%c'\n", c);
				show_help = true;
				return (is_valid = false);
				break;
		}
	}

	if (new_argv) {
		freeargv(new_argv);
		new_argv = NULL;
	}

	if (argv)
		OPTIONS->PrintOptions(argv[0]);

	return IsValid();
}

bool GameOptions::IsValid() {
	if (!is_valid)
		return false;

	return (is_valid = true);
}

GameOptions::~GameOptions()	{}
GameOptions::GameOptions() 	{ Clear(); }
