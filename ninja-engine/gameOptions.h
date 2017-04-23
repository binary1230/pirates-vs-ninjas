// reads command line arguments and parse them

#ifndef ARGS_H
#define ARGS_H

//! Holds and validates the various command line and game options
class GameOptions {

	DECLARE_SINGLETON_CLASS(GameOptions)

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
		std::string demo_filename;

		//! Whether to enable sound or not
		bool sound_enabled;

		//! Requested graphics mode (double buffering, pageflipping, nothing, etc)
		int graphics_mode;

		//! The first mode we should use (leave "" to select the default mode)
		std::string first_mode;

		//! If true, start the game in debug pause mode
		bool debug_start_paused;

		//! Debug message level (default is DEFAULT_DEBUG_MSG_LEVEL)
		int debug_message_level;

		//! Network server to connect to
		std::string network_server_name;

		//! Network port to use
		int network_port_num;

		//! True if we are starting the game as a server
		bool network_start_as_server;

		//! True if networking is enabled
		bool network_enabled;

		//! Whether to do any drawing at all.  If false, no window appears
		//! (Useful for running AI training faster)
		bool draw_graphics;

		//! Whether we maintain a constant FPS or just 
		//! run the game as fast as we can.
		//! For normal play, this should be true
		//! For AI training or testing, you can set this to false to run fast
		bool wait_for_updates;
		
	public:
		//! Print the list of available options to stdout
		void PrintOptions(const char* arg0);

		//! Print the banner for the game to stdout
		void PrintBanner();


		//! Parse the command line arguments
		bool ParseArguments(const int argc, const char* argv[]);

		//! See if the current state of our options make sense
		bool IsValid();

		//! Clear the members of this class
		void Clear();

		
		//! True if the combinations of options we have selected is valid
		inline bool IsFullscreen() {return fullscreen;}
		
		//! The demo filename if it was set
		inline const char* GetDemoFilename() { return demo_filename.c_str(); }
		
		//! True if the user wants to reccord a demo
		inline bool RecordDemo() {return record_demo;}
		
		//! True if the user wants to playback a demo
		inline bool PlaybackDemo() {return playback_demo;}
		
		//! Requested graphics mode (double buffering, pageflipping, nothing, etc)
		//! (used for the Window class)
		inline int GraphicsMode() {return graphics_mode;}

		//! Get the user-overridden mode XML filename, or "" if none
		inline const char* GetFirstModeOverride() {
			if (first_mode.length() > 0)
				return first_mode.c_str();
		 	else 
				return NULL;
		}

		//! Get whether sound is enabled or not
		inline bool SoundEnabled() {return sound_enabled;}
		
		//! Return whether we are starting the game in 'debug pause mode'
		inline bool GetDebugStartPaused() {return debug_start_paused;}

		//! Return the debug message level.
		//! Right now, 0 = no messages, 1 = all messages
		inline int GetDebugMessageLevel() {return debug_message_level;}

		inline bool		IsNetworkEnabled() {return network_enabled;}
		inline bool 	IsNetworkServer() {return network_start_as_server;}
		inline int 		GetNetworkPortNumber() {return network_port_num;}
		inline const char* GetNetworkServerName() {
			return network_server_name.c_str();
		}

		inline bool		DrawGraphics() {return draw_graphics;}
		inline bool		WaitForUpdates() {return wait_for_updates;}
		
		virtual ~GameOptions();
};

#define OPTIONS (GameOptions::GetInstance())

#endif // ARGS_H
