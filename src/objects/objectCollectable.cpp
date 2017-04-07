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
