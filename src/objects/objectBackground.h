#ifndef BackgroundObject_H
#define BackgroundObject_H

#include "stdafx.h"
#include "object.h"

class BackgroundObject;
class GameState;
class ObjectFactory;
class PhysSimulation;

//! An Object which repeats itself, usually used for backgrounds
class BackgroundObject : public Object {
	protected:
				
	public:
		bool Init(PhysSimulation *p);
		void Shutdown();

		void Update();
		void Draw();

		BackgroundObject();
		~BackgroundObject();

		friend class ObjectFactory;
};

#endif // BackgroundObject_H   
