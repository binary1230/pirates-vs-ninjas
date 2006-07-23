#include "objectCollectable.h"

#include "globals.h"
#include "animation.h"
#include "gameState.h"

void CollectableObject::Update() {
	if (currentAnimation) {
		currentAnimation->Update();
		currentSprite = currentAnimation->GetCurrentSprite();
	}
}

bool CollectableObject::Init(GameState *_game_state) {
	SetGameState(_game_state);
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

Object* CollectableObject::New(	GameState* gameState, 
																XMLNode &xDef, 
																XMLNode &xObj) {
				
	CollectableObject* obj = new CollectableObject();

	if (!obj || !obj->Init(gameState) )
		return NULL;

	if (!obj->LoadSounds(xDef))
		return NULL;
	
	if (!obj->LoadAnimations(xDef))
		return NULL;
	
	if (!obj->LoadProperties(xDef))
		return NULL;

	obj->properties.is_collectable = 1;

	return obj;
}
