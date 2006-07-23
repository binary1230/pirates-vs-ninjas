#ifndef COLLECTABLE_OBJECT_H
#define COLLECTABLE_OBJECT_H

#include <stdio.h>

class GameState;

#include "object.h"
#include "xmlParser.h"

//! A "simple" Object (e.g. scenery) - No collisions
class CollectableObject : public Object {
	protected:
				
	public:
		bool Init(GameState* _game_state);
		
		void Update();

		CollectableObject();
		~CollectableObject();

		void Collide(Object* obj);
			
		static Object* New(GameState* gameState, XMLNode &xDef, XMLNode &xObj);
};

#endif // CollectableObject_H   
