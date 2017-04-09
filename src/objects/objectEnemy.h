#ifndef ENEMY_OBJECT_H
#define ENEMY_OBJECT_H

#include "object.h"

class ObjectFactory;
class PlayerObject;

//! An enemy object that interacts with the player
class EnemyObject : public Object {
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Object);
		// ar & BOOST_SERIALIZATION_NVP(a_var_you_want_to_serialize);
	}

	protected:
		void MoveTowardsPlayer();
		PlayerObject* m_pkTargetPlayer;

		bool bCollidedLastFrame;
		b2Vec2 m_kCollisionDirection;
		int iTimeToWaitBeforeCollisionsAllowedAgain;
				
	public:
		bool Init();
		void Shutdown();
		
		void Update();

		EnemyObject();
		virtual ~EnemyObject();

		virtual void OnCollide(Object* obj, const b2WorldManifold* pkbWorldManifold);

		static int iSpawnedObjectCount;

		friend class ObjectFactory;
};

#endif // EnemyObject_H   
