#include "objectStatic.h"

#include "globals.h"
#include "animation.h"
#include "gameState.h"

void StaticObject::Shutdown() {
	BaseShutdown();
}

void StaticObject::Update() {
	BaseUpdate();
	UpdateSimpleAnimations();
}

bool StaticObject::Init(PhysSimulation *p) {
	simulation = p;
	return BaseInit();
}

StaticObject::StaticObject() {}
StaticObject::~StaticObject() {}
