#ifndef SPRING_OBJECT_H
#define SPRING_OBJECT_H

#include <stdio.h>

class GameState;

#include "object.h"
#include "xmlParser.h"

//! A "simple" Object (e.g. scenery) - No collisions
class SpringObject : public Object {
	protected:
				
	public:
		bool Init(GameState* _game_state);
		
		void Update();

		SpringObject();
		~SpringObject();

		void Collide(Object* obj);
			
		static Object* New(GameState* gameState, XMLNode &xDef, XMLNode &xObj);
};

#endif // SpringObject_H   
