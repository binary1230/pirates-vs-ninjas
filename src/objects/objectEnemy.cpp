#include "objectEnemy.h"

#include "globals.h"
#include "animation.h"
#include "gameState.h"

void EnemyObject::Update() {
	UpdateFade();
	if (currentAnimation) {
		currentAnimation->Update();
		currentSprite = currentAnimation->GetCurrentSprite();
	}
}

bool EnemyObject::Init(GameState *_game_state, PhysSimulation *p) {
	SetGameState(_game_state);
	simulation = p;
	return BaseInit();
}

EnemyObject::EnemyObject() {}
EnemyObject::~EnemyObject() {}

void EnemyObject::Collide(Object* obj) {
	if (obj->GetProperties().is_player) {
		// PlaySound("ring");
		// is_dead = true;
	}
}
