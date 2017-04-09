#ifndef COLLECTABLE_OBJECT_H
#define COLLECTABLE_OBJECT_H

#include "object.h"

class GameState;
class ObjectFactory;

//! A "simple" Object (e.g. scenery) - No collisions
class CollectableObject : public Object {
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Object);
		// ar & BOOST_SERIALIZATION_NVP(a_var_you_want_to_serialize);
	}

	protected:
				
	public:
		bool Init();
		void Shutdown();
		
		void Update();

		CollectableObject();
		~CollectableObject();

		virtual void OnCollide(Object* obj, const b2WorldManifold* pkbWorldManifold);
	
		friend class ObjectFactory;
};

#endif // CollectableObject_H   
