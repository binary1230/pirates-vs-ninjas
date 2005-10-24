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

class ObjectFactory : GameBase {
	protected:
		BITMAP* default_destination_bitmap;
		BaseInput* input;

	public:
		int Init(GameState* _game_state, BaseInput* handler);
		void Shutdown();

		Object* CreateObject(uint id);
		void DeleteObject(Object*);
	  
		void SetDefaultDestinationBitmap(BITMAP* bmp);
		void SetInputHandler(BaseInput* handler);
		
		ObjectFactory();
		~ObjectFactory();
};

#endif // OBJECT_FACTORY_H
