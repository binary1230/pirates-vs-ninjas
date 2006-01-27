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
			//! The mode to go to on gameState->NextMode()
			GameMode* next;		
			
			//! The mode to go to on gameState->ExitMode()
			GameMode* parent; 

		public:
			virtual int Init(GameState*, XMLNode) = 0;
			virtual void Shutdown() = 0;

			//! Draw this mode
			virtual void Draw() = 0;

			//! Update this mode
			virtual void Update() = 0;

			//! Set the next mode to the following (or NULL)
			inline void SetNextMode(GameMode* n) {next = n;};

			//! Get the next game mode
			inline GameMode* GetNextMode() {return next;};
			
			//! Set the parent mode to the following
			inline void SetParentMode(GameMode* p) {parent = p;};

			//! Get the parent mode
			inline GameMode* GetParentMode() {return parent;};

			GameMode();
			virtual ~GameMode();
};

#endif // GAMEMODE_H
