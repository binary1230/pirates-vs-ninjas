#ifndef MODELOBJECT_H
#define MODELOBJECT_H

#include "stdafx.h"
#include "object.h"

class ObjectFactory;
class PhysSimulation;
class Model_3DS;

//! A "simple" Object (e.g. scenery) - No collisions
class ModelObject : public Object {
	protected:
		Model_3DS* model;
				
	public:
		bool Init(PhysSimulation *p);
		void Shutdown();
		
		void Update();
		void Draw();

		ModelObject();
		~ModelObject();
			
		friend class ObjectFactory;
};

#endif // ModelObject_H   
