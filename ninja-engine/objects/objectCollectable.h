#ifndef COLLECTABLE_OBJECT_H
#define COLLECTABLE_OBJECT_H

#include "object.h"
#include "globals.h"

class GameState;

class ObjectCollectable : public Object {
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Object);
	}

	protected:
				
	public:
		IMPLEMENT_CLONE(ObjectCollectable)

		bool Init();
		virtual void ResetVolatileState(VolatileStateLevel level);
		void Shutdown();
		
		void Update();

		virtual bool LoadObjectProperties(XMLNode & xDef);

		ObjectCollectable();
		~ObjectCollectable();

		virtual void OnCollide(Object* obj, const b2WorldManifold* pkbWorldManifold);

		CREATE_PROPERTY(bool, Consumed);
	
		
};

#endif // CollectableObject_H   
