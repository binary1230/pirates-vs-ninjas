#ifndef BOUNCE_OBJECT_H
#define BOUNCE_OBJECT_H

#include "stdafx.h"
#include "object.h"

class ObjectFactory;
class PhysSimulation;

class ObjectBounce : public Object {
	protected:
		bool play_hit_sound;
		bool collided_last_frame;

	public:
		bool Init(PhysSimulation *p);
		void Shutdown();
		void Update();

		void Collide(Object* obj);
		
		ObjectBounce();
		~ObjectBounce();
			
		//! Factory method to create a new radius block object
		friend class ObjectFactory;
};

#endif // ObjectBounce_H   
