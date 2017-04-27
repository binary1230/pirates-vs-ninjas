#include "stdafx.h"
#include <math.h>
#include "objectFan.h"

#include "globals.h"
#include "animation.h"
#include "gameState.h"
#include "objectPlayer.h"

// all of this class is slightly hackish for the moment.
#define FAN_DECAY_RATE 0.96f
#define MAX_PLAYER_SPEED 10.0f
#define DEFAULT_FAN_VELOCITY 30.0f

void ObjectFan::Shutdown() {
	BaseShutdown();
}

void ObjectFan::Update() {
	_use_rotation = true;
	_rotate_velocity *= FAN_DECAY_RATE;
	
	BaseUpdate();
	UpdateSimpleAnimations();
}

void ObjectFan::OnCollide(Object* obj, const b2WorldManifold* pkbWorldManifold) {
	if (ObjectPlayer* player = dynamic_cast<ObjectPlayer*>(obj)) {

		// if the player is going slowly, slow down the new fan speed
		float player_factor = std::min(	MAX_PLAYER_SPEED, (float)fabs(obj->GetVelX()) ); 
		player_factor /= MAX_PLAYER_SPEED;

		// figure out which way to spin the fan (left/right)
		float direction = -1.0f;
		if (obj->GetVelX() < 0)
			direction = 1.0f;

		// calc the new rotational velocity
		_rotate_velocity = direction * std::max(DEFAULT_FAN_VELOCITY*player_factor, (float)fabs(_rotate_velocity));
	}
}

bool ObjectFan::LoadObjectProperties(XMLNode &xDef) {
	if (!Object::LoadObjectProperties(xDef))
		return false;

	properties.uses_physics_engine = 1;
	properties.is_static = 1;
	properties.is_sensor = 1;
	properties.do_our_own_rotation = 1;

	return true;
}

bool ObjectFan::Init() {
	return BaseInit();
}

void ObjectFan::Clear() {

}

ObjectFan::ObjectFan() { Clear(); }
ObjectFan::~ObjectFan() {}

BOOST_CLASS_EXPORT_GUID(ObjectFan, "ObjectFan")