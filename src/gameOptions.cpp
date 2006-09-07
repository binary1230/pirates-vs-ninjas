/* This file contains code taken from OpenDarwin Project:
   Create and destroy argument vectors (argv's)
   Copyright (C) 1992, 2001 Free Software Foundation, Inc.
   Written by Fred Fish @ Cygnus Support

   This file is part of the libiberty library. (GPL)*/

#include "gameOptions.h"
#include "basename.h"
#include "window.h"
#include "globals.h"

void GameOptions::PrintBanner() {
		fprintf(stderr, 
		"Ninja Engine (%s)\n"
		"binary1230(at)yahoo.com | http://einsteinsbreakfast.com\n"
		"(c) 2005 Dominic Cerquetti, this program is Free Software\n"
		"Licensed under the GNU GPL v2, see http://gnu.org\n\n",
		VERSION_STRING);
}

void GameOptions::PrintOptions(const char* arg0) {
	arg0 = basename(arg0);
	if (!show_help) {
		fprintf(stderr, "type '%s -h' for more options..\n\n", arg0);
	} else {
		fprintf(stderr,
		"NOTE: data files MUST be in a dir in THIS folder called 'data'\n\n"

		"Usage: %s [options]\n"
		"-f            | try to force fullscreen mode\n"
		"-w            | try to force windowed mode\n"
		"-g mode       | 0 = no buffering, 1 = double buffer [default]\n"
		"              | 2 = page flipping, 3 = triple buffering\n\n"
		
		"-m id         | specify a different mode ID to use from default.xml\n\n"
		
		"-r file       | record a demo to 'file'\n"
		"-d file       | playback a demo from 'file'\n\n"

		"-X            | disable sound\n\n"

		"-2            | (DEBUG) use 640x480 instead of 320x240\n"
		"-3            | (DEBUG) start the game paused (press F1 and F2 in game)\n"
		"-v            | (DEBUG) show debugging messages\n\n"

		"-s            | (EXPERIMENTAL) start game as a server\n\n"
		"-c servername | (EXPERIMENTAL) connect to game server [servername]\n"
		"-p portnumber | (EXPERIMENTAL) connect to game server port\n\n"

		"-8            | Run game as fast as possible (not for human playing)\n"
		"-9            | Do not do anything interactive (not for human playing)\n"

		"-h            | display this help message\n\n"

		, arg0);
	}
}

void GameOptions::Clear() {
	fullscreen = false;
	show_help = false;
	
	default_mode_id = 0;

	record_demo = false;
	playback_demo = false;
	
	demo_filename = "";
	
	sound_enabled = 1;
	
	debug_start_paused = false;
	debug_message_level = DEFAULT_DEBUG_MSG_LEVEL;

	graphics_mode = MODE_DOUBLEBUFFERING;	

	network_enabled = 0;
	network_port_num = -1;
	network_server_name = "";
	network_start_as_server = false;

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

bool GameOptions::ParseArguments(const int argc, const char* argv[]) {
	
	char c;
	bool _fullscreen_option_set = false;
	char** new_argv = dupargv(argv);
	
	if (!new_argv) {
		return (is_valid = false);
	}

	Clear();

	while ( (c = getopt(argc,new_argv,"fwg:m:r:d:X23vsc:p:h89")) != -1) {
		switch (c) {

			case 'm':
				default_mode_id = strtoul(optarg, NULL, 10);
				break;

			// get demo filename
			case 'r': case 'd':
				if (demo_filename.length() > 0) {
						fprintf(stderr,	"Options ==> ERROR "
														"Don't give more than 1 demo filename (-r, -d)\n");
						return (is_valid = false);
				}

				if (c == 'r')
						record_demo = true;
				else 
						playback_demo = true;
								
				demo_filename = CString(optarg);
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
					fprintf(stderr,"Options ==> ERROR, Cannot select both fullscreen (-f) and windowed (-w) mode.\n");
					return (is_valid = false);
				} else if (c == 'f') {
					fullscreen = true;
					_fullscreen_option_set = true;
					fprintf(stderr, "Options ==> fullscreen mode enabled\n");
				} else if (c == 'w') {
					fullscreen = false;
					_fullscreen_option_set = true;
					fprintf(stderr, "Options ==> windowed mode enabled\n");
				}
				break;

			case 'g':
				graphics_mode = strtoul(optarg, NULL, 10);
				if (graphics_mode >= 4 || graphics_mode < 0) {
					fprintf(stderr, "ERROR: Graphics mode is out of range.\n");
					show_help = true;
					return (is_valid = false);
				}
				break;

			// debug: hack - '2x size'
			case '2':
				screen_size_x = 640;
				screen_size_y = 480;
				break;

			// debug: start in 'paused' mode, press F1 to go, F2 to step
			case '3':
				debug_start_paused = true;
				break;

			// debug: enable extra debug messages
			case 'v':
				debug_message_level = 1;
				break;
	
			// Network: Start game as server
			case 's':
				network_start_as_server = true;
				network_enabled = true;
				break;
	
			// Network server name
			case 'c':
				network_server_name = CString(optarg);
				network_enabled = true;
				break;
	
			// Network port #
			case 'p':
				network_port_num = strtoul(optarg, NULL, 10);
				network_enabled = true;
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
				fprintf(stderr, "Unrecognized command line option '-%c'\n", c);
				show_help = true;
				return (is_valid = false);
				break;
		}
	}

	if (new_argv) {
		freeargv(new_argv);
		new_argv = NULL;
	}

	return IsValid();
}

bool GameOptions::IsValid() {

	if (!is_valid)
		return false;

	if (network_enabled) {

		if (network_port_num == -1) {
			fprintf(	stderr, 
								"Options ==> ERROR\n You MUST specify a port with (-p).\n\n");
			return (is_valid = false);
		} else if (network_port_num <= 0) {
			fprintf(	stderr, 
								"Options ==> ERROR\n (-p) Port # out of range.\n\n");
			return (is_valid = false);
		}


		// if we aren't to start as a server, but they didn't give us a server name
		// (btw, ^ is XOR) [YES.]
		if (!(network_start_as_server ^ network_server_name.GetLength() > 0)) {
			fprintf(	stderr, "Options ==> ERROR\n"
												"To start with networking, you must specify ONLY ONE\n"
												"of the following: (-c) or (-s servername)\n\n");
			return (is_valid = false);
		}
	}

	return (is_valid = true);
}

GameOptions::~GameOptions()	{}
GameOptions::GameOptions() 	{ Clear(); }
