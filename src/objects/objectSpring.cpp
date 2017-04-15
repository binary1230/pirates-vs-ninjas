#include "stdafx.h"
#include "objectSpring.h"

#include "globals.h"
#include "animation.h"
#include "gameState.h"
#include "gameSound.h"
#include "physics.h"

#define DEFAULT_SPRING_RESET_TIME 6

void SpringObject::Shutdown() {
	BaseShutdown();
}

void SpringObject::Update() {
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

bool SpringObject::LoadObjectProperties(XMLNode &xDef) {
	if (!Object::LoadObjectProperties(xDef))
		return false;

	properties.is_spring = 1;
	properties.uses_physics_engine = 1;
	properties.is_static = 1;
	properties.is_sensor = 1;

	return true;
}

bool SpringObject::Init() {
	spring_reset_time = 0;
	spring_is_active = true;

	return BaseInit();
}

SpringObject::SpringObject() {}
SpringObject::~SpringObject() {}

void SpringObject::OnCollide(Object* obj, const b2WorldManifold* pkbWorldManifold) {
	if (obj->GetProperties().is_player) {
		
		// Spring it!
		if (spring_reset_time == 0) {
			currentAnimation->Unfreeze();
			spring_reset_time = DEFAULT_SPRING_RESET_TIME;
		}
	}
}

BOOST_CLASS_EXPORT_GUID(SpringObject, "SpringObject")