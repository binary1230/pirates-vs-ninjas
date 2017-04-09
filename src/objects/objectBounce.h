#ifndef BOUNCE_OBJECT_H
#define BOUNCE_OBJECT_H

#include "object.h"

class ObjectFactory;

class ObjectBounce : public Object {
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Object);
		// ar & BOOST_SERIALIZATION_NVP(a_var_you_want_to_serialize);
	}

	protected:
		bool play_hit_sound;
		bool collided_last_frame;

	public:
		bool Init();
		void Shutdown();
		void Update();

		virtual void OnCollide(Object* obj, const b2WorldManifold* pkbWorldManifold);
		
		ObjectBounce();
		~ObjectBounce();
			
		//! Factory method to create a new radius block object
		friend class ObjectFactory;
};

#endif // ObjectBounce_H   
