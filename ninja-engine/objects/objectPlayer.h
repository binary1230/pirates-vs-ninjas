#ifndef PLAYEROBJECT_H
#define PLAYEROBJECT_H

#include "globals.h"
#include "object.h"

class Animation;
class BaseInput;
class Animation;
class BaseInput;
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
class ObjectPlayer : public Object {
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Object);
		ar & BOOST_SERIALIZATION_NVP(controller_num);
	}

	protected:
		float jump_velocity;
		float min_velocity;
		float drag;

		//! Which controller (e.g. which joystick) use, if we are getting
		//! input for this object
		int controller_num;

		// How long until we are allowed to draw another "skid" object
		int next_skid_time;

		// What we're currently doing
		PlayerState m_kPlayerState;

		//! position saved from when we were loaded, useful for map editor and such.
		b2Vec2 _pos_at_load;

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
		IMPLEMENT_CLONE(ObjectPlayer)

		virtual bool Init();
		virtual void Shutdown();

		virtual void Update();
		virtual void OnCollide(Object* obj, const b2WorldManifold* pkbWorldManifold);

		void OnItemPickup(const string & item_name);

		virtual void ResetVolatileState(VolatileStateLevel level);

		virtual void OnAnimationLooped();
		virtual void PlayAnimation(uint uiIndex);
		
		int GetNumRings() {return ring_count;};
			
		ObjectPlayer();
		virtual ~ObjectPlayer();

		bool WantsToSlideOnLeftSide();
		bool WantsToSlideOnRightSide();

		bool WantsToSlideOnAnySide();
};

#if !defined(SWIG) 
BOOST_CLASS_VERSION(ObjectPlayer, 1)
#endif // SWIG

#endif // PLAYER_OBJECT_H
