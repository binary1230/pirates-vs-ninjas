// GameState
// represents the state of the game
#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <allegro.h>
#include <stdio.h>
#include <vector>

using namespace std;

#include "window.h"
#include "object.h"
#include "objectFactory.h"

class GameState {
	protected:
		Window *window;
		ObjectFactory *objectFactory;

		vector<Object*> objects;
		
		bool exit_game;
					
		enum state {
			STATE_NORMAL,
			STATE_PAUSED
		};
					
		int InitSystem();
		
		int InitObjects();
		void DestroyObjects();
		
		void Shutdown();

		void MainLoop();

		void Update();
		void Draw();
				
	public:
		int RunGame(/* options */);

		GameState();
		~GameState();
};

#endif
