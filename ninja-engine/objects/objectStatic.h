#ifndef STILLOBJECT_H
#define STILLOBJECT_H

#include "object.h"

class ObjectFactory;

//! A "simple" Object (e.g. scenery) - No collisions
class ObjectStatic : public Object {
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Object);
	}

	protected:
		
				
	public:
		IMPLEMENT_CLONE(ObjectStatic)

		virtual bool LoadObjectProperties(XMLNode & xDef);
		bool Init();
		void Shutdown();
		
		void Update();

		ObjectStatic();
		~ObjectStatic();

		void UpdateSpawns();

		friend class ObjectFactory;
};

#endif // StaticObject_H   
