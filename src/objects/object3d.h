#ifndef MODELOBJECT_H
#define MODELOBJECT_H

#include "object.h"

class ObjectFactory;
class Model_3DS;

//! A "simple" Object (e.g. scenery) - No collisions
class ModelObject : public Object {
	protected:
		Model_3DS* model;
				
	public:
		bool Init();
		void Shutdown();
		
		void Update();
		void Draw();

		ModelObject();
		~ModelObject();
			
		friend class ObjectFactory;
};

#endif // ModelObject_H   
