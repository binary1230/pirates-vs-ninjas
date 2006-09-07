#ifndef GAMEBASE_H
#define GAMEBASE_H

#include "stdafx.h"

class GameState;

//! A generic base class for all game classes
class GameBase {
		protected:
			//! A pointer to the current game state
			GameState* game_state;
	
		public:
			//! Returns the game state
			inline GameState* GetGameState() {
				return game_state;
			};

			//! Sets the game state
			inline void SetGameState(GameState* _game_state) {
				game_state = _game_state;
			};

			//! Display 'not supported'
			inline bool NotSupported() {
				fprintf(stderr, "Not supported.\n");
				return false;
			}

			GameBase();
			virtual ~GameBase();
};

#endif //  GAMEBASE_H
