#include "stdafx.h"
#include "objectCollectable.h"

#include "globals.h"
#include "animation.h"
#include "gameState.h"
#include "gameSound.h"
#include "objectPlayer.h"

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

	_Properties.uses_physics_engine = 1;
	_Properties.is_static = 1;
	_Properties.is_sensor = 1;

	return true;
}

bool ObjectCollectable::Init() {
	return BaseInit();
}

void ObjectCollectable::ResetVolatileState(VolatileStateLevel level) {
	if (level >= LEVEL_ITEMS) {
		_Consumed = false;
		_dont_draw = false;
	}
}

ObjectCollectable::ObjectCollectable() 
{
	_Consumed = false;
}
ObjectCollectable::~ObjectCollectable() {}

void ObjectCollectable::OnCollide(Object* obj, const b2WorldManifold* pkbWorldManifold) {
	if (_Consumed)
		return;

	if (ObjectPlayer* player = dynamic_cast<ObjectPlayer*>(obj)) {
		SOUND->PlaySound("ring");
		_Consumed = true;
		_dont_draw = true;
	}
}

BOOST_CLASS_EXPORT_GUID(ObjectCollectable, "ObjectCollectable")