// this file is a TEST ONLY

#ifndef PLAYEROBJECT_H
#define PLAYEROBJECT_H

#include <allegro.h>
#include <math.h>
#include <stdio.h>
#include <vector>

using namespace std;

class ObjectPlayer;
class Animation;
class GameState;
class BaseInput;
class Animation;

#include "object.h"
#include "xmlParser.h"

//! The Player object, represents our HERO on screen

//! Quite possibly the most important class in the game
class PlayerObject : public Object {
	protected:
		float jump_velocity;
		float min_velocity;
		float drag;
				
	public:
		bool Init(GameState* _game_state);
		
		bool LoadProperties(XMLNode &xDef);		//! Load object properties from XML
		
		void Update();
			
		static Object* New(GameState* gameState, XMLNode &xDef, XMLNode &xObj);
		
		PlayerObject();
		~PlayerObject();
};

#endif // PLAYER_OBJECT_H
