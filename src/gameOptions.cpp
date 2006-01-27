#include "gameOptions.h"
#include "basename.h"
#include "globals.h"

void GameOptions::PrintBanner() {
		fprintf(stderr, 
		"Ninja Engine (%s)\n"
		"binary1230(at)yahoo.com | http://einsteinsbreakfast.com\n"
		"(c) 2005 Dominic Cerquetti, this program is Free Software\n"
		"Licensed under the GNU GPL v2, see http://gnu.org\n\n",
		VERSION_STRING);
}

void GameOptions::PrintOptions(char* arg0) {
	arg0 = basename(arg0);
	if (!show_help) {
		fprintf(stderr, "type '%s -h' for more options..\n\n", arg0);
	} else {
		fprintf(stderr,
		"NOTE: .dat files MUST be in a dir in THIS folder called 'data'\n\n"

		"Usage: %s [options]\n"
		"-f            | try to force fullscreen mode\n"
		"-w            | try to force windowed mode\n"
		"-g mode       | 0 = no buffering, 1 = double buffer [default]\n"
		"              | 2 = page flipping, 3 = triple buffering\n\n"
		
		"-r file       | record a demo to 'file'\n"
		"-p file       | playback a demo from 'file'\n\n"

		"-h            | display this help message\n\n"
		, arg0);
	}
}

void GameOptions::Clear() {
	fullscreen = false;
	show_help = false;

	record_demo = false;
	playback_demo = false;
	
	demo_filename = NULL;

	graphics_mode = 1;	// XXX magic number. get it from window.h

	is_valid = true;
}

bool GameOptions::ParseArguments(int argc, char* argv[]) {
	
	char c;
	char* buffer;
	bool _fullscreen_option_set = false;

	Clear();

	while ( (c = getopt(argc,argv,"g:r:p:fwh")) != -1) {
		switch (c) {

			// get demo filename
			case 'r': case 'p':
				if (demo_filename) {
						fprintf(stderr,	"Options ==> ERROR "
														"Don't give more than 1 demo filename (-r, -p)\n");
						return (is_valid = false);
				}

				if (c == 'r')
						record_demo = true;
				else 
						playback_demo = true;
								
				buffer = new char[strlen(optarg) + 1];
				demo_filename = strcpy(buffer, optarg);
				break;
			
			// display help
			case 'h':
				show_help = true;
				return (is_valid = false);
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
				// XXX Magic number.  #define u twit.
				if (graphics_mode >= 4 || graphics_mode < 0) {
					fprintf(stderr, "ERROR: Graphics mode is out of range.\n");
					show_help = true;
					return (is_valid = false);
				}
				break;
	
			// : and ? mean something is weird
			default:
			case ':': case '?':
				fprintf(stderr, "Unrecognized command line option '-%c'\n", c);
				show_help = true;
				return (is_valid = false);
				break;
		}
	}
	return IsValid();
}

bool GameOptions::IsValid() {
	return is_valid;
}

GameOptions::~GameOptions() {
	if (demo_filename)
		delete demo_filename;
}

GameOptions::GameOptions() : demo_filename(NULL) {}
