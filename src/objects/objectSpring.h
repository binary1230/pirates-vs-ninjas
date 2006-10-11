#ifndef SPRING_OBJECT_H
#define SPRING_OBJECT_H

#include "stdafx.h"

#include "object.h"

class ObjectFactory;
class PhysSimulation;

//! A "simple" Object (e.g. scenery) - No collisions
class SpringObject : public Object {
	protected:
		uint time_to_wait_before_sound;
				
	public:
		bool Init(PhysSimulation *p);
		void Shutdown();
		void Update();

		SpringObject();
		~SpringObject();

		void Collide(Object* obj);
			
		friend class ObjectFactory;

};

#endif // SpringObject_H   
