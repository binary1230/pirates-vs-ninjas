#ifndef BackgroundObject_H
#define BackgroundObject_H

#include <math.h>
#include <stdio.h>

class BackgroundObject;
class GameState;
class ObjectFactory;
class PhysSimulation;

#include "object.h"
#include "xmlParser.h"

//! An Object which repeats itself, usually used for backgrounds
class BackgroundObject : public Object {
	protected:
				
	public:
		bool Init(GameState* _game_state, PhysSimulation *p);

		void Update();
		void Draw();

		BackgroundObject();
		~BackgroundObject();

		friend class ObjectFactory;
};

#endif // BackgroundObject_H   
