#ifndef GAMESTATE_H
#define GAMESTATE_H


#include <allegro.h>
#include <stdio.h>
#include <vector>

using namespace std;

class GameState;

#include "globals.h"
#include "gameOptions.h"
#include "timer.h"
#include "input.h"
#include "inputLiveHandler.h"
#include "inputRecord.h"
#include "inputPlayback.h"
#include "object.h"
#include "objectFactory.h"
#include "window.h"
#include "force.h"

class Window;

//! Represents the current state of the game.

//! This is the 'main object' for the game - the centrally
//! coordinated object which initializes, runs, and destroys
//! the entire game.  Anything of importance starts in this
//! class somewhere.
class GameState {
	protected:

		//! Random seed for the game
		int random_seed;
					
		//! Options parsed from the commandline
		GameOptions *options;
		
		//! The game's on-screen window
		Window *window;								
		
		//! Creates new objects
		ObjectFactory *objectFactory;	
		
		//! Active input module
		BaseInput *input;

		//! Collection of all drawable objects
		vector<Object*> objects;

		//! Collection of forces
		vector<Force*> forces;
	
		//! Set this to TRUE to exit the game 
		bool exit_game;
			
		//! The state of the game engine
		enum state {
			STATE_NORMAL,
			STATE_PAUSED
		};
					
		int InitSystem();
		
		int InitTimers();
		int InitInput();
		int InitObjects();
		
		void DestroyObjects();
		
		void Shutdown();

		void MainLoop();

		void Update();
		void Draw();

				
	public:
		int RunGame(GameOptions *_options);

		// accessors
		// inline BaseInput* 		GetInput() 					{ return input; };
		// inline Window* 				GetWindow() 				{ return window; };
		inline GameOptions* 	GetGameOptions() 		{ return options; };
		inline ObjectFactory*	GetObjectFactory()	{ return objectFactory; };

		inline void SetRandomSeed(int val)		{ random_seed = val; srand(val); };
		inline int GetRandomSeed()						{ return random_seed; };

		//! Get status of a key
		inline bool GetKey(uint which_key)				{ return input->Key(which_key); };

		//! Get drawing surface (essentially the screen)
		BITMAP* GetDrawingSurface();
		uint Width();
		uint Height();

		GameState();
		~GameState();
};

#endif
