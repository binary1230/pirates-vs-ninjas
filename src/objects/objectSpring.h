#ifndef SPRING_OBJECT_H
#define SPRING_OBJECT_H

#include "stdafx.h"

#include "object.h"

class GameState;
class ObjectFactory;
class PhysSimulation;

//! A "simple" Object (e.g. scenery) - No collisions
class SpringObject : public Object {
	protected:
				
	public:
		bool Init(GameState* _game_state, PhysSimulation *p);
		void Update();

		SpringObject();
		~SpringObject();

		void Collide(Object* obj);
			
		friend class ObjectFactory;

};

#endif // SpringObject_H   
