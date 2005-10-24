#ifndef GAMEBASE_H
#define GAMEBASE_H

#include <stdio.h>

//! Really generic base class for all game classes

class GameState;

class GameBase {
		protected:
			GameState* game_state;
	
		public:
			inline GameState* GetGameState() {
				return game_state;
			};

			inline void SetGameState(GameState* _game_state) {
				game_state = _game_state;
			};

			GameBase();
			virtual ~GameBase();
};

#endif //  GAMEBASE_H
