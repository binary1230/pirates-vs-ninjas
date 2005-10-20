#include "gameOptions.h"

void GameOptions::PrintUsage(char* arg0) {
				
	fprintf(stderr, 
		"Dom's Ninja-Engine %s [CVS REVISION %s]\n"
		"binary1230(at)yahoo.com | http://einsteinsbreakfast.com\n"
		"(c) 2005 Dominic Cerquetti, this program is Free Software\n"
		"Licensed under the GNU GPL v2, see http://gnu.org\n\n",
		ENGINE_VERSION, SUBVERSION_REVISION);

	if (!show_help) {
		fprintf(stderr, "type '%s -h' for more options..\n\n", arg0);
	} else {
		fprintf(stderr,
		"NOTE: .dat files MUST be in a dir in THIS folder called 'data'\n\n"

		"Usage: %s [options]\n"
		"-f            | try to force fullscreen mode\n"
		"-w            | try to force windowed mode\n"
		"-h            | display this help message\n\n"
		, arg0);
	}
}

void GameOptions::Clear() {
	fullscreen = false;
	show_help = false;

	is_valid = true;
}

bool GameOptions::ParseArguments(int argc, char* argv[]) {
	
	char c;
	bool _fullscreen_option_set = false;

	Clear();

	while ( (c = getopt(argc,argv,"fwh")) != -1) {
		switch (c) {
			
			// display help
			case 'h':
				show_help = true;
				return (is_valid = false);
				break;
	
			// fullscreen or windowed
			case 'f': case 'w':
				if (_fullscreen_option_set) {
					fprintf(stderr,"GFX: Use one of -f or -w, not both.\n");
					return (is_valid = false);
				} else if (c == 'f') {
					fullscreen = true;
					_fullscreen_option_set = true;
					fprintf(stderr, "GFX: fullscreen mode enabled\n");
				} else if (c == 'f') {
					fullscreen = false;
					_fullscreen_option_set = true;
					fprintf(stderr, "GFX: windowed mode enabled\n");
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

bool GameOptions::IsFullscreen() {
	return fullscreen;
}
