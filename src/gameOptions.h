// reads command line arguments and parse them

#ifndef ARGS_H
#define ARGS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

//! Holds and validates the various command line and game options
class GameOptions {
	protected:
		//! True if the combinations of options we have selected is valid
		bool is_valid;
		
		//! True if we are to show the help message instead of running the game
		bool show_help;

		//! True if we are to attempt fullscreen display
		bool fullscreen;
	

		//! True if the user wants to reccord a demo
		bool record_demo;

		//! True if the user wants to playback a demo
		bool playback_demo;

		//! The demo filename if it was set
		char* demo_filename;

		//! Requested graphics mode (double buffering, pageflipping, nothing, etc)
		int graphics_mode;

		//! The default mode ID to use (leave 0 to select the first mode)
		int default_mode_id;

		//! If true, start the game in debug pause mode
		bool debug_start_paused;
		
	public:
		//! Print the list of available options to stdout
		void PrintOptions(char* arg0);

		//! Print the banner for the game to stdout
		void PrintBanner();


		//! Parse the command line arguments
		bool ParseArguments(int argc, char* argv[]);

		//! See if the current state of our options make sense
		bool IsValid();

		//! Clear the members of this class
		void Clear();

		
		//! True if the combinations of options we have selected is valid
		inline bool IsFullscreen() {return fullscreen;};
		
		//! The demo filename if it was set
		inline char* GetDemoFilename() {return demo_filename;};
		
		//! True if the user wants to reccord a demo
		inline bool RecordDemo() {return record_demo;};
		
		//! True if the user wants to playback a demo
		inline bool PlaybackDemo() {return playback_demo;};
		
		//! Requested graphics mode (double buffering, pageflipping, nothing, etc)
		//! (used for the Window class)
		inline int GraphicsMode() {return graphics_mode;};

		//! Get the default mode ID
		inline int GetDefaultModeId() {return default_mode_id;};
		
		//! Return whether we are starting the game in 'debug pause mode'
		inline bool GetDebugStartPaused() {return debug_start_paused;};
		
		GameOptions();
		~GameOptions();
};

#endif // ARGS_H
