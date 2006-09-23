#ifndef STILLOBJECT_H
#define STILLOBJECT_H

#include "stdafx.h"
#include "object.h"

class ObjectFactory;
class PhysSimulation;

//! A "simple" Object (e.g. scenery) - No collisions
class StaticObject : public Object {
	protected:
				
	public:
		bool Init(PhysSimulation *p);
		
		void Update();

		StaticObject();
		~StaticObject();
			
		friend class ObjectFactory;
};

#endif // StaticObject_H   
