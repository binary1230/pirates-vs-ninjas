#include "objectStatic.h"

#include "globals.h"
#include "animation.h"
#include "gameState.h"

void StaticObject::Update() {
	BaseUpdate();
	UpdateSimpleAnimations();
}

bool StaticObject::Init(GameState *_game_state, PhysSimulation *p) {
	SetGameState(_game_state);
	simulation = p;
	return BaseInit();
}

StaticObject::StaticObject() {}
StaticObject::~StaticObject() {}
