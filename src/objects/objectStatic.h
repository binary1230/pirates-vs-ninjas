#ifndef STILLOBJECT_H
#define STILLOBJECT_H

#include "object.h"

class ObjectFactory;

//! A "simple" Object (e.g. scenery) - No collisions
class StaticObject : public Object {
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

		StaticObject();
		~StaticObject();

		void UpdateSpawns();

		friend class ObjectFactory;

		int junk_test;
};

#endif // StaticObject_H   
