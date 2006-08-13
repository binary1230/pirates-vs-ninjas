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
class ObjectFactory;
class PhysSimulation;
class DoorObject;

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
	CROUCHINGDOWN,
	WALKING_THRU_DOOR
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
		void DoWalkThroughDoor();

		void ScreenBoundsConstraint();
		void UpdateSpriteFlip();
		void UpdateRunningAnimationSpeed();
		void DoCommonGroundStuff();
		
		DoorObject* door_in_front_of_us;

	public:
		bool Init(GameState* _game_state, PhysSimulation* p);
		
		//! Load object properties from XML
		bool LoadPlayerProperties(XMLNode &xDef);		
		
		void Update();
		void Collide(Object* obj);
			
		PlayerObject();
		~PlayerObject();

		friend class ObjectFactory;
};

#endif // PLAYER_OBJECT_H
