#include "stdafx.h"
#include "objectBounce.h"

#include "globals.h"
#include "animation.h"
#include "game.h"
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

	if (alpha == 0) {
		is_dead = true;
	}

	if (hit_with_explosion_last_frame) {
		_physics_body->SetType(b2_dynamicBody);
		DontCollideWithPlayer();
		FadeOut(60);
	}
	
	hit_with_explosion_last_frame = false;
}

bool ObjectBounce::LoadObjectProperties(XMLNode &xDef) {
	if (!Object::LoadObjectProperties(xDef))
		return false;

	uses_physics_engine = 1;

	XMLNode xProps = xDef.getChildNode("properties");
	_static_until_heavy_impact = xProps.nChildNode("staticUntilHeavyImpact") != 0;
	
	return true;
}

bool ObjectBounce::Init() {
	play_hit_sound = false;
	hit_with_explosion_last_frame = false;

	return BaseInit();
}

void ObjectBounce::Clear() {
	Object::Clear();
	_static_until_heavy_impact = false;
}

void ObjectBounce::InitPhysics() {
	Object::InitPhysics();

	if (_static_until_heavy_impact)
		_physics_body->SetType(b2_staticBody);
}

void ObjectBounce::OnCollide(Object* obj, const b2WorldManifold* pkbWorldManifold) {
	bool isExplosion = !obj; // this... won't be true forever. fix this for real.

	if (isExplosion)
		hit_with_explosion_last_frame = true;
}

ObjectBounce::ObjectBounce() {}
ObjectBounce::~ObjectBounce() {}

BOOST_CLASS_EXPORT_GUID(ObjectBounce, "ObjectBounce")