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
class BaseInput;

#include "object.h"
#include "xmlParser.h"

enum PlayerState {
	STANDING,
	JUMPING,
	FALLING,
	WALKING,
	RUNNING,
	WHISTLING,
	LOOKINGUP,
	CROUCHINGDOWN
};

//! The Player object, represents our HERO on screen
class PlayerObject : public Object {
	protected:
		float jump_velocity;
		float min_velocity;
		float drag;

		PlayerState state;
		BaseInput* input;
	
		void UpdateState();

		void DoStanding();
		void DoWalking();
		void DoRunning();
		void DoJumping();
		void DoFalling();
		void DoWhistling();
		void DoLookingUp();
		void DoCrouchingDown();

		void ScreenBoundsConstraint();
		void UpdateSpriteFlip();
		void UpdateRunningAnimationSpeed();
		void DoCommonGroundStuff();

	public:
		bool Init(GameState* _game_state);
		
		bool LoadPlayerProperties(XMLNode &xDef);		//! Load object properties from XML
		
		void Update();
		void Collide(Object* obj);
			
		static Object* New(GameState* gameState, XMLNode &xDef, XMLNode &xObj);
		
		PlayerObject();
		~PlayerObject();
};

#endif // PLAYER_OBJECT_H
