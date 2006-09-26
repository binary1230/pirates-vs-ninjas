#ifndef BOUNCE_OBJECT_H
#define BOUNCE_OBJECT_H

#include "stdafx.h"
#include "object.h"

class ObjectFactory;
class PhysSimulation;

class BounceObject : public Object {
	protected:

	public:
		bool Init(PhysSimulation *p);
		void Update();

		void Collide(Object* obj);
		
		BounceObject();
		~BounceObject();
			
		//! Factory method to create a new radius block object
		friend class ObjectFactory;
};

#endif // BounceObject_H   
