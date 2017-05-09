#include "stdafx.h"
#include <math.h>
#include "objectFan.h"

#include "globals.h"
#include "animation.h"
#include "game.h"
#include "objectPlayer.h"

// all of this class is slightly hackish for the moment.
#define FAN_DECAY_RATE 0.96f
#define MAX_PLAYER_SPEED 10.0f
#define DEFAULT_FAN_VELOCITY 30.0f

void ObjectFan::Shutdown() {
	BaseShutdown();
}

void ObjectFan::Update() {
	_RotateVelocity *= FAN_DECAY_RATE;
	
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
		_RotateVelocity = direction * std::max(DEFAULT_FAN_VELOCITY*player_factor, (float)fabs(_RotateVelocity));
	}
}

bool ObjectFan::LoadObjectProperties(XMLNode &xDef) {
	if (!Object::LoadObjectProperties(xDef))
		return false;

	uses_physics_engine = 1;
	is_static = 1;
	is_sensor = 1;

	return true;
}

bool ObjectFan::Init() {
	return BaseInit();
}

void ObjectFan::Clear() {
	Object::Clear();
}

ObjectFan::ObjectFan() { Clear(); }
ObjectFan::~ObjectFan() {}

BOOST_CLASS_EXPORT_GUID(ObjectFan, "ObjectFan")