// GameState
// represents the state of the game
#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "window.h"

class GameState {
	protected:
		Window *window;
		
		bool exit_game;
					
		enum state {
			STATE_NORMAL,
			STATE_PAUSED
		};
					
		int Init();
		void Shutdown();

		void Update();

		void Draw();
				
	public:
		int RunGame(/* options */);

		GameState();
		~GameState();
};

#endif
