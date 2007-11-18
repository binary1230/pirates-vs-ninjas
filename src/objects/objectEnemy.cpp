#include "stdafx.h"
#include "objectEnemy.h"

#include "globals.h"
#include "animation.h"
#include "gameState.h"

void EnemyObject::Shutdown() {
	BaseShutdown();
}

void EnemyObject::Update() {
	BaseUpdate();
	SimpleAnimationUpdate();
}

bool EnemyObject::Init() {
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
