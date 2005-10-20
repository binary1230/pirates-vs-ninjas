// reads command line arguments and parse them

#ifndef ARGS_H
#define ARGS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "globals.h"

class GameOptions {
	protected:
		bool is_valid;

		bool fullscreen;
		
	public:
		void PrintUsage(char* arg0, bool full = false);
		bool ParseArguments(int argc, char* argv[]);
		bool IsValid();
		void Clear();

		bool IsFullscreen();
};

#endif // ARGS_H
