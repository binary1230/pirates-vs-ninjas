#ifndef BOUNCE_OBJECT_H
#define BOUNCE_OBJECT_H

#include "object.h"

class ObjectFactory;

class ObjectBounce : public Object {
	protected:
		bool play_hit_sound;
		bool collided_last_frame;

	public:
		bool Init();
		void Shutdown();
		void Update();

		void OnCollide(Object* obj, const b2WorldManifold* pkbWorldManifold);
		
		ObjectBounce();
		~ObjectBounce();
			
		//! Factory method to create a new radius block object
		friend class ObjectFactory;
};

#endif // ObjectBounce_H   
