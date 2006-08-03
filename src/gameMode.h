#ifndef GAMEMODE_H
#define GAMEMODE_H

class GameState;

#include "gameBase.h"
#include "xmlParser.h"

//! Base class for all Modes of the game (e.g. Menu Mode, Game Mode, Bonus Level Mode)

//! For example, a level is a "mode", the menu screen is a "mode"
//! and the bonus stage is a "mode"
//! 
//! This class doesn't do anything except act as an interface to enforce
//! the fact that all modes need to be able to Update() and Draw(), among
//! other things.  It also acts as a node in a linked list in order to hold
//! all the game modes.
class GameMode : public GameBase {
		protected:

		public:
			virtual int Init(GameState*, XMLNode) = 0;
			virtual void Shutdown() = 0;

			//! Draw this mode
			virtual void Draw() = 0;

			//! Update this mode
			virtual void Update() = 0;

			GameMode();
			virtual ~GameMode();
};

#endif // GAMEMODE_H
