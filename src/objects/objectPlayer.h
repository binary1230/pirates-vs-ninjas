#ifndef PLAYEROBJECT_H
#define PLAYEROBJECT_H

#include "object.h"

class ObjectPlayer;
class Animation;
class BaseInput;
class Animation;
class BaseInput;
class ObjectFactory;
class ObjectDoor;

enum PlayerState {
	STANDING,
	JUMPING,
	FALLING,
	WALKING_THRU_DOOR,
	SLIDING_DOWN_WALL,
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
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Object);
	}

	protected:
		float jump_velocity;
		float min_velocity;
		float drag;

		// How long until we are allowed to draw another "skid" object
		int next_skid_time;

		// What we're currently doing
		PlayerState m_kPlayerState;

		ObjectDoor* door_in_front_of_us;
		int ring_count;

		bool m_bShouldNotSwitchAnimationsRightNow;

		void Clear();

		virtual bool LoadObjectProperties(XMLNode & xDef);

		void DropBombsIfNeeded();

		void ScreenBoundsConstraint();
		void UpdateSpriteFlip();
		void UpdateRunningAnimationSpeed();
		void UpdateLeftRightMotion();

		bool GetInput(uint key, uint controller_number) const;

	public:
		virtual bool Init();
		virtual void Shutdown();

		virtual void Update();
		virtual void OnCollide(Object* obj, const b2WorldManifold* pkbWorldManifold);

		virtual void OnAnimationLooped();
		virtual void PlayAnimation(uint uiIndex);
		
		int GetNumRings() {return ring_count;};
			
		PlayerObject();
		virtual ~PlayerObject();

		bool WantsToSlideOnLeftSide();
		bool WantsToSlideOnRightSide();

		bool WantsToSlideOnAnySide();

		friend class ObjectFactory;
};

#endif // PLAYER_OBJECT_H
