#include "objectSpring.h"

#include "globals.h"
#include "animation.h"
#include "gameState.h"

void SpringObject::Update() {
	if (currentAnimation) {
		currentAnimation->Update();
		currentSprite = currentAnimation->GetCurrentSprite();
	}
}

bool SpringObject::Init(GameState *_game_state) {
	SetGameState(_game_state);
	return BaseInit();
}

SpringObject::SpringObject() {}
SpringObject::~SpringObject() {}

void SpringObject::Collide(Object* obj) {
	if (obj->GetProperties().is_player) {
		currentAnimation->Unfreeze();
	}
}

Object* SpringObject::New(	GameState* gameState, 
																XMLNode &xDef, 
																XMLNode &xObj) {
				
	SpringObject* obj = new SpringObject();

	if (!obj || !obj->Init(gameState) )
		return NULL;

	if (!obj->LoadAnimations(xDef))
		return NULL;
	
	if (!obj->LoadProperties(xDef))
		return NULL;

	obj->properties.is_spring = 1;

	return obj;
}
