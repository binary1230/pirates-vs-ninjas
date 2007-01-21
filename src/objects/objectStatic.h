#ifndef STILLOBJECT_H
#define STILLOBJECT_H

#include "stdafx.h"
#include "object.h"

class ObjectFactory;

//! A "simple" Object (e.g. scenery) - No collisions
class StaticObject : public Object {
	protected:
				
	public:
		bool Init();
		void Shutdown();
		
		void Update();

		StaticObject();
		~StaticObject();
			
		friend class ObjectFactory;
};

#endif // StaticObject_H   
