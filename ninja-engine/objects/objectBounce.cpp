#include "stdafx.h"
#include "objectBounce.h"

#include "globals.h"
#include "animation.h"
#include "gameState.h"
#include "gameWorld.h"
#include "gameSound.h"
#include "physics.h"

#define MIN_VELOCITY 0.34f
#define FRICTION_MULTIPLIER 0.70f

void ObjectBounce::Shutdown() {
	BaseShutdown();
}

void ObjectBounce::Update() {
	BaseUpdate();
	UpdateSimpleAnimations();

	if (play_hit_sound) {
		// SOUND->PlaySound("ball_hit");
		play_hit_sound = false;
	}
	
	collided_last_frame = m_kCurrentCollision.down;
}

bool ObjectBounce::LoadObjectProperties(XMLNode &xDef) {
	if (!Object::LoadObjectProperties(xDef))
		return false;

	_Properties.uses_physics_engine = 1;
	
	return true;
}

bool ObjectBounce::Init() {
	play_hit_sound = false;
	collided_last_frame = false;

	return BaseInit();
}

void ObjectBounce::OnCollide(Object* obj, const b2WorldManifold* pkbWorldManifold) {

}

ObjectBounce::ObjectBounce() {}
ObjectBounce::~ObjectBounce() {}

BOOST_CLASS_EXPORT_GUID(ObjectBounce, "ObjectBounce")