#ifndef ENEMY_OBJECT_H
#define ENEMY_OBJECT_H

#include "stdafx.h"
#include "object.h"

class ObjectFactory;

//! An enemy object that interacts with the player
class EnemyObject : public Object {
	protected:
				
	public:
		bool Init();
		void Shutdown();
		
		void Update();

		EnemyObject();
		~EnemyObject();

		void Collide(Object* obj);
	
		friend class ObjectFactory;
};

#endif // EnemyObject_H   
