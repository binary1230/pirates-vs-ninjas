#ifndef FORCE_FACTORY_H
#define FORCE_FACTORY_H

#include "allegro_wrapper.h"
#include <stdio.h>

class Force;
class GameState;

#include "gameBase.h"
#include "force.h"

//! A class which produces new Force objects from ID's
class ForceFactory : public GameBase {
	protected:

	public:
		int Init(GameState* _game_state);
		void Shutdown();

		Force* CreateForce(ForceType type);
		void DeleteForce(Force*);
		
		ForceFactory();
		~ForceFactory();
};

#endif // FORCE_FACTORY_H
