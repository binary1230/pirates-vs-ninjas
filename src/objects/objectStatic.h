#ifndef STILLOBJECT_H
#define STILLOBJECT_H

#include <stdio.h>

class GameState;
class ObjectFactory;
class PhysSimulation;

#include "object.h"
#include "xmlParser.h"

//! A "simple" Object (e.g. scenery) - No collisions
class StaticObject : public Object {
	protected:
				
	public:
		bool Init(GameState* _game_state, PhysSimulation *p);
		
		void Update();

		StaticObject();
		~StaticObject();
			
		friend class ObjectFactory;
};

#endif // StaticObject_H   
