#include "stdafx.h"
#include "objectStatic.h"

#include "globals.h"
#include "animation.h"
#include "gameState.h"

bool StaticObject::Init() {
	return BaseInit();
}

void StaticObject::Shutdown() {
	BaseShutdown();
}

void StaticObject::Update() {
	BaseUpdate();
	UpdateSimpleAnimations();
}

StaticObject::StaticObject() {}
StaticObject::~StaticObject() {}
