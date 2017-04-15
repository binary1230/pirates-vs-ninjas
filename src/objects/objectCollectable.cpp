#include "stdafx.h"
#include "objectCollectable.h"

#include "globals.h"
#include "animation.h"
#include "gameState.h"
#include "gameSound.h"

void CollectableObject::Shutdown() {
	BaseShutdown();
}

void CollectableObject::Update() {
	BaseUpdate();
	UpdateSimpleAnimations();
}

bool CollectableObject::LoadObjectProperties(XMLNode &xDef) {
	if (!Object::LoadObjectProperties(xDef))
		return false;

	properties.is_collectable = 1;
	properties.is_ring = 1;
	properties.uses_physics_engine = 1;
	properties.is_static = 1;
	properties.is_sensor = 1;

	return true;
}

bool CollectableObject::Init() {
	return BaseInit();
}

CollectableObject::CollectableObject() {}
CollectableObject::~CollectableObject() {}

void CollectableObject::OnCollide(Object* obj, const b2WorldManifold* pkbWorldManifold) {
	if (obj->GetProperties().is_player) {
		SOUND->PlaySound("ring");
		is_dead = true;
	}
}

BOOST_CLASS_EXPORT_GUID(CollectableObject, "CollectableObject")