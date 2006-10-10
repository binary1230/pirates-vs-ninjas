#include "objectSpring.h"

#include "globals.h"
#include "animation.h"
#include "gameState.h"
#include "gameSound.h"

void SpringObject::Update() {
	BaseUpdate();
	UpdateSimpleAnimations();
}

bool SpringObject::Init(PhysSimulation *p) {
	simulation = p;
	return BaseInit();
}

SpringObject::SpringObject() {}
SpringObject::~SpringObject() {}

void SpringObject::Collide(Object* obj) {
	if (obj->GetProperties().is_player) {
		currentAnimation->Unfreeze();
		SOUND->PlaySound("spring");
	}
}


