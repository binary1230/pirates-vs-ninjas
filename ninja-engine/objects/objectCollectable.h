#ifndef COLLECTABLE_OBJECT_H
#define COLLECTABLE_OBJECT_H

#include "object.h"
#include "globals.h"

class Game;
class ObjectPlayer;

class ObjectCollectable : public Object {
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Object);
	}

	protected:
		string _pickup_type;
		string _sound_on_pickup;

		void OnCollideWithPlayer(ObjectPlayer * player);
				
	public:
		IMPLEMENT_CLONE(ObjectCollectable)

		string GetPickupSound() { return _sound_on_pickup; }
		string GetPickupType() { return _pickup_type; }

		bool Init();
		virtual void ResetVolatileState(VolatileStateLevel level);
		void Shutdown();

		virtual void Clear();
		
		void Update();

		virtual bool LoadObjectProperties(XMLNode & xDef);

		ObjectCollectable();
		~ObjectCollectable();

		virtual void OnCollide(Object* obj, const b2WorldManifold* pkbWorldManifold);

		CREATE_PROPERTY(bool, Consumed);
	
		
};

#endif // CollectableObject_H   
