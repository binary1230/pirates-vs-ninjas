// this file is a TEST ONLY

#ifndef PLAYEROBJECT_H
#define PLAYEROBJECT_H

#include <allegro.h>
#include <math.h>
#include <stdio.h>
#include <vector>

using namespace std;

class ObjectPlayer;

#include "globals.h"
#include "object.h"
#include "input.h"
#include "inputLiveHandler.h"
#include "force.h"
#include "forceGravity.h"
#include "forceInput.h"
#include "vec.h"

// directions
#define _dX 0
#define _dY 1

class PlayerObject : public Object {
	protected:
		BaseInput* input;
		vec vectors[2];		// vectors for X and Y
				
	public:
		void Update();
		void Draw();
		void SetInputHandler(BaseInput* handler);

		void SetXY(int, int);

		bool Init(GameState* _game_state);
		void Shutdown();

		PlayerObject();
		~PlayerObject();
};

#endif // PLAYER_OBJECT_H
