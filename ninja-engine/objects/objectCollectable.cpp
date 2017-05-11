#include "stdafx.h"
#include "objectCollectable.h"

#include "globals.h"
#include "animation.h"
#include "game.h"
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

	_pickup_type = xDef.getChildNode("item_type").getText();
	_sound_on_pickup = xDef.getChildNode("sound_on_pickup").getText();

	uses_physics_engine = 1;
	is_static = 1;
	is_sensor = 1;

	return true;
}

void ObjectCollectable::Clear() {
	Object::Clear();

	_pickup_type = "";
	_sound_on_pickup = "";
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
		player->OnItemPickup(_pickup_type);

		SOUND->PlaySound(_sound_on_pickup);

		_Consumed = true;
		_dont_draw = true;
	}
}

BOOST_CLASS_EXPORT_GUID(ObjectCollectable, "ObjectCollectable")