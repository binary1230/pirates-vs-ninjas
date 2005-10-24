#ifndef BackgroundObject_H
#define BackgroundObject_H

#include <math.h>
#include <stdio.h>

class BackgroundObject;

#include "globals.h"
#include "object.h"

class BackgroundObject : public Object {
	protected:
				
	public:
		bool Init(GameState* _game_state);
		void Shutdown();

		void Update();
		void Draw();

		BackgroundObject();
		~BackgroundObject();
};

#endif // BackgroundObject_H   
