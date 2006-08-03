#include "objectCollectable.h"

#include "globals.h"
#include "animation.h"
#include "gameState.h"

void CollectableObject::Update() {
	UpdateFade();
	if (currentAnimation) {
		currentAnimation->Update();
		currentSprite = currentAnimation->GetCurrentSprite();
	}
}

bool CollectableObject::Init(GameState *_game_state, PhysSimulation *p) {
	SetGameState(_game_state);
	simulation = p;
	return BaseInit();
}

CollectableObject::CollectableObject() {}
CollectableObject::~CollectableObject() {}

void CollectableObject::Collide(Object* obj) {
	if (obj->GetProperties().is_player) {
		PlaySound("ring");
		is_dead = true;
	}
}
