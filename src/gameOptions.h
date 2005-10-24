// reads command line arguments and parse them

#ifndef ARGS_H
#define ARGS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

class GameOptions;

#include "globals.h"

class GameOptions {
	protected:
		bool is_valid;
		
		bool show_help;
		bool fullscreen;
		
	public:
		void PrintOptions(char* arg0);
		void PrintBanner();

		bool ParseArguments(int argc, char* argv[]);
		bool IsValid();
		void Clear();

		bool IsFullscreen();
};

#endif // ARGS_H
