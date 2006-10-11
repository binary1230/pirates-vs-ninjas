#ifndef DOOROBJECT_H
#define DOOROBJECT_H

#include "stdafx.h"

#include "object.h"

class ObjectFactory;
class PhysSimulation;

class DoorObject : public Object {
	protected:
		// e.g. What action happens on Activate()
	
	public:
		bool Init(PhysSimulation *p);
		void Shutdown();
		
		void Update();

		// Activate this door's action.
		void Activate();

		DoorObject();
		~DoorObject();
			
		friend class ObjectFactory;
};

#endif // DoorObject_H   
