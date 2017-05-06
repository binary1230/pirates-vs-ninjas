#ifndef BOUNCE_OBJECT_H
#define BOUNCE_OBJECT_H

#include "object.h"

class ObjectBounce : public Object {
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Object);
	}

	protected:
		bool play_hit_sound;
		bool hit_with_explosion_last_frame;

		bool _static_until_heavy_impact;

	public:
		IMPLEMENT_CLONE(ObjectBounce)

		virtual bool Init();
		virtual void Clear();
		virtual void Shutdown();
		virtual void Update();

		virtual void InitPhysics();

		virtual bool LoadObjectProperties(XMLNode & xDef);

		virtual void OnCollide(Object* obj, const b2WorldManifold* pkbWorldManifold);
		
		ObjectBounce();
		~ObjectBounce();
			
		//! Factory method to create a new radius block object
		
};

#endif // ObjectBounce_H   
