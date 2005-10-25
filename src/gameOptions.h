// reads command line arguments and parse them

#ifndef ARGS_H
#define ARGS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

class GameOptions;

#include "globals.h"

class GameOptions {
	protected:
		bool is_valid;
		
		bool show_help;
		bool fullscreen;
	
		bool record_demo;
		bool playback_demo;
		char* demo_filename;
		
	public:
		void PrintOptions(char* arg0);
		void PrintBanner();

		bool ParseArguments(int argc, char* argv[]);
		bool IsValid();
		void Clear();

		inline bool IsFullscreen() {return fullscreen;};
		inline char* GetDemoFilename() {return demo_filename;};
		inline bool RecordDemo() {return record_demo;};
		inline bool PlaybackDemo() {return playback_demo;};

		GameOptions();
		~GameOptions();
};

#endif // ARGS_H
