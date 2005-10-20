#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <allegro.h>
#include <stdio.h>
#include <vector>

using namespace std;

#include "gameOptions.h"

#include "window.h"
#include "timer.h"
#include "input.h"
#include "inputLiveHandler.h"

#include "object.h"
#include "objectFactory.h"

//! Represents the current state of the game.

//! This is the 'main object' for the game - the centrally
//! coordinated object which initializes, runs, and destroys
//! the entire game.  Anything of importance starts in this
//! class somewhere.
class GameState {
	protected:
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
	
		//! Set this to TRUE to exit the game 
		bool exit_game;
			
		//! The state of the game engine
		enum state {
			STATE_NORMAL,
			STATE_PAUSED
		};
					
		int InitSystem();
		int InitTimers();

		int InitObjects();
		void DestroyObjects();
		
		void Shutdown();

		void MainLoop();

		void Update();
		void Draw();
				
	public:
		int RunGame(GameOptions *_options);

		GameState();
		~GameState();
};

#endif
