#ifndef PLAYEROBJECT_H
#define PLAYEROBJECT_H

#include "stdafx.h"
#include "object.h"

class ObjectPlayer;
class Animation;
class BaseInput;
class Animation;
class BaseInput;
class ObjectFactory;
class PhysSimulation;
class DoorObject;

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
	
		// How long until we are allowed to draw another "skid" object
		int next_skid_time;

		PlayerState state;
	
		void UpdateState();

		void DoCommonStuff();

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
		void UpdateSkidding();
		
		DoorObject* door_in_front_of_us;
		int ring_count;

		// If the running animation is a skateboard (only set at init time)
		bool on_skateboard;

	public:
		bool Init(PhysSimulation* p);
		void Shutdown();
		
		//! Load object properties from XML
		bool LoadPlayerProperties(XMLNode &xDef);		
		
		void Update();
		void Collide(Object* obj);
		
		int GetNumRings() {return ring_count;};
			
		PlayerObject();
		~PlayerObject();

		friend class ObjectFactory;
};

#endif // PLAYER_OBJECT_H
