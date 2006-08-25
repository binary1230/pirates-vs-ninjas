#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "allegro_wrapper.h"
#include <stdio.h>
#include "stl_wrapper.h"

class GameState;
class GameOptions;
class BaseInput;
class Window;
class GameMode;
class AssetManager;
class PhysSimulation;
class GameSound;
class GameModes;
class ezSockets;
class GameNetwork;

#include "globals.h"
#include "timer.h"
#include "StdString.h"
#include "xmlParser.h"

//! Represents the current state of the game.

//! The MAIN class in the game - the centrally
//! coordinated object which initializes, runs, and destroys
//! the entire game.  Anything of importance starts in this
//! class somewhere.
class GameState {
	protected:	
		//! Holds options which were parsed from the command line
		GameOptions *options;
	
		//! Holds the current game's parsed XML data
		XMLNode xGame;
		
		//! Loads things and knows about path handling
		AssetManager* assetManager;
		
		//! The game's on-screen window
		Window *window;								
				
		//! The active input class
		BaseInput *input;

		//! The sound class
		GameSound *sound;

		//! Collection of all game modes (e.g. menu mode, simulation mode, etc)
		GameModes *modes;

		//! The game network functionality:
		GameNetwork *network;

		//! True if we are using the network
		bool IsUsingNetwork() {return network != NULL;};
		
		//! Initialize all game related stuff
		int InitSystem();								
		
		//! Init basic allegro stuff
		int InitAllegro();

		//! Init game timing
		int InitTimers();
		
		//! Init input subsystems
		int InitInput();								

		//! Init sound system
		int InitSound();

		//! Init the game network
		int InitNetwork();
		
		//! Load master game config from an XML file, return an XMLNode which is
		//! the first game Mode's config we should now load.
		int LoadXMLConfig(CString xml_filename);
		
		//! Load a game mode from an XML config file
		int LoadGameModes();

		//! Load the default configuration
		int LoadDefaultConfig();				
		
		//! Shutdown the game
		void Shutdown();

		//! Main loop to call once everything is initialized
		void MainLoop();

		//! Update the state of the game
		void Update();

		//! Draw the current state of the game
		void Draw();
		
		//! Set to TRUE to immediately shutdown the game 
		bool exit_game;	
	
		//! Flag to enable DEBUG pause toggling
		bool debug_pause_toggle;

		//! If a game has the same random seed, it will
		//! return the EXACT same sequence of random numbers.
		//! (useful for recording demos)
		int random_seed;
	
		//! Set to true if we are playing back a demo
		bool is_playing_back_demo;
					
	public:
		//! Main function - run the game
		int RunGame(GameOptions *_options);

		//! Set the random seed value
 		void SetRandomSeed(int);
		
		//! Get the random seed value
		int GetRandomSeed() const;

		//! Returns true if we are playing back a demo
		bool IsPlayingBackDemo() {return is_playing_back_demo;};

		//! Return a surface which we can draw on
		BITMAP* GetDrawingSurface();

		//! Returns true if the given key is being pressed, false otherwise
		bool GetKey(uint which_key) const;

		//! Returns true if the given key is being pressed 
		//! on a given controller, false otherwise
		bool GetKey(uint which_key, uint which_controller) const;
	
		//! Return screen width
		uint ScreenWidth()  const;
		
		//! Return screen height
		uint ScreenHeight() const;

		//! Get our resource loader.
		inline AssetManager* GetAssetManager() const {return assetManager;};

		//! Unconditionally exit game
		void SignalGameExit();
		
		//! The current mode calls this to signal it wants to end
		void SignalEndCurrentMode();

		//! Returns the currently active physics simulation (if there is one)
		PhysSimulation* GetPhysSimulation();

		//! Get the current window
		inline Window* GetWindow() {return window;};

		//! Gets the game options
		inline GameOptions* GetGameOptions() {return options;};

		//! Gets the sound class
		inline GameSound* GetSound() {return sound;};
		
		//! Gets the input class
		inline BaseInput* GetInput() {return input;};

		GameState();
		~GameState();
};

#endif
