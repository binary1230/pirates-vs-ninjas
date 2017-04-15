#include "stdafx.h"
#include "objectCollectable.h"

#include "globals.h"
#include "animation.h"
#include "gameState.h"
#include "gameSound.h"

void ObjectCollectable::Shutdown() {
	BaseShutdown();
}

void ObjectCollectable::Update() {
	BaseUpdate();
	UpdateSimpleAnimations();
}

bool ObjectCollectable::LoadObjectProperties(XMLNode &xDef) {
	if (!Object::LoadObjectProperties(xDef))
		return false;

	properties.is_collectable = 1;
	properties.is_ring = 1;
	properties.uses_physics_engine = 1;
	properties.is_static = 1;
	properties.is_sensor = 1;

	return true;
}

bool ObjectCollectable::Init() {
	return BaseInit();
}

ObjectCollectable::ObjectCollectable() {}
ObjectCollectable::~ObjectCollectable() {}

void ObjectCollectable::OnCollide(Object* obj, const b2WorldManifold* pkbWorldManifold) {
	if (obj->GetProperties().is_player) {
		SOUND->PlaySound("ring");
		is_dead = true;
	}
}

BOOST_CLASS_EXPORT_GUID(ObjectCollectable, "ObjectCollectable")