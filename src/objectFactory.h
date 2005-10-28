#ifndef OBJECT_FACTORY_H
#define OBJECT_FACTORY_H

#include <allegro.h>
#include <stdio.h>

class ObjectFactory;

#include "globals.h"
#include "object.h"
#include "objectIDs.h"

#include "objectPlayer.h"
#include "objectRadiusBlock.h"
#include "objectBackground.h"

#include "gameBase.h"

#include "platform.h"

// THIS CLASS IS BAD.
// It will be rewritten, for now it JUST WORKS.
class ObjectFactory : GameBase {
	protected:

	public:
		int Init(GameState* _game_state);
		void Shutdown();

		Object* CreateObject(uint id);
		void DeleteObject(Object*);
		
		ObjectFactory();
		~ObjectFactory();
};

#endif // OBJECT_FACTORY_H
