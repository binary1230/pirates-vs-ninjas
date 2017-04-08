#ifndef ENEMY_OBJECT_H
#define ENEMY_OBJECT_H

#include "object.h"

class ObjectFactory;
class PlayerObject;

//! An enemy object that interacts with the player
class EnemyObject : public Object {
	protected:
		void MoveTowardsPlayer();
		PlayerObject* m_pkTargetPlayer;

		bool bCollidedLastFrame;
		Vector2D m_kCollisionDirection;
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
