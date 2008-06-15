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

// TODO: rename when it becomes more apparent what this is describing
enum InputStateMask {
	INPUT_NOTHING	= 0x0,
	INPUT_JUMP		= 0x1,
	INPUT_ACTION1	= 0x2,
	INPUT_LEFT		= 0x4,
	INPUT_RIGHT		= 0x8,
	INPUT_UP		= 0x10,
	INPUT_DOWN		= 0x20,
};

//! The Player object, represents our HERO on screen
class PlayerObject : public Object {
	protected:
		float jump_velocity;
		float min_velocity;
		float drag;
	
		// How long until we are allowed to draw another "skid" object
		int next_skid_time;

		// What we're currently doing
		PlayerState m_kPlayerState;

		void HandleInput();
	
		void UpdateState();

		void DoCommonStuff();

		void DropBombs();

		void DoStanding();
		void DoWalking();
		void DoRunning();
		
		void DoCommonAirStuff();
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

		virtual bool GetInput(uint key, uint controller_number) const = 0;
		
		DoorObject* door_in_front_of_us;
		int ring_count;

		// If the running animation is a skateboard (only set at init time)
		bool on_skateboard;

	public:
		bool Init();
		void Shutdown();
		
		//! Load object properties from XML
		bool LoadPlayerProperties(XMLNode &xDef);		
		
		void Update();
		void OnCollide(Object* obj, const b2ContactPoint* pkContactPoint);
		
		int GetNumRings() {return ring_count;};
			
		PlayerObject();
		virtual ~PlayerObject();

		friend class ObjectFactory;
};

#endif // PLAYER_OBJECT_H
