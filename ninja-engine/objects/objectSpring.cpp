#include "stdafx.h"
#include "objectSpring.h"

#include "globals.h"
#include "animation.h"
#include "gameState.h"
#include "gameSound.h"
#include "physics.h"
#include "objectPlayer.h"

#define DEFAULT_SPRING_RESET_TIME 6

void ObjectSpring::Shutdown() {
	BaseShutdown();
}

void ObjectSpring::Update() {
	BaseUpdate();
	UpdateSimpleAnimations();

	if (spring_reset_time == DEFAULT_SPRING_RESET_TIME)
		spring_is_active = false;

	if (spring_reset_time > 0) {
		spring_reset_time--;
		return;
	}

	if (spring_reset_time == 0) {
		spring_is_active = true;
	}

	/*else
		spring_is_active = true;

	// We were just sprung last Collide()
	if (!spring_is_active && spring_reset_time == 0)
		spring_reset_time = DEFAULT_SPRING_RESET_TIME;*/
}

bool ObjectSpring::LoadObjectProperties(XMLNode &xDef) {
	if (!Object::LoadObjectProperties(xDef))
		return false;

	_Properties.uses_physics_engine = 1;
	_Properties.is_static = 1;
	_Properties.is_sensor = 1;

	return true;
}

bool ObjectSpring::Init() {
	spring_reset_time = 0;
	spring_is_active = true;

	return BaseInit();
}

void ObjectSpring::Clear() {
	_Direction.x = DEFAULT_SPRING_STRENGTH_X;
	_Direction.y = DEFAULT_SPRING_STRENGTH_Y;
}

ObjectSpring::ObjectSpring() { Clear(); }
ObjectSpring::~ObjectSpring() {}

void ObjectSpring::OnCollide(Object* obj, const b2WorldManifold* pkbWorldManifold) {
	if (ObjectPlayer* player = dynamic_cast<ObjectPlayer*>(obj)) {
		if (spring_reset_time == 0) {
			currentAnimation->Unfreeze();
			spring_reset_time = DEFAULT_SPRING_RESET_TIME;
		}
	}
}

BOOST_CLASS_EXPORT_GUID(ObjectSpring, "ObjectSpring")