#ifndef STILLOBJECT_H
#define STILLOBJECT_H

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

		void UpdateSpawns();

		friend class ObjectFactory;
};

#endif // StaticObject_H   
