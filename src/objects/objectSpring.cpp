#include "objectSpring.h"

#include "globals.h"
#include "animation.h"
#include "gameState.h"
#include "gameSound.h"

#define DEFAULT_SPRING_RESET_TIME 6

void SpringObject::Shutdown() {
	BaseShutdown();
}

void SpringObject::Update() {
	BaseUpdate();
	UpdateSimpleAnimations();

	if (spring_reset_time == DEFAULT_SPRING_RESET_TIME)
		spring_is_active = false;

	if (spring_reset_time > 0) {
		spring_reset_time--;
		return;
	} 
	
	if (spring_reset_time == 0) {
		spring_is_active = true;
	}

	/*else
		spring_is_active = true;

	// We were just sprung last Collide()
	if (!spring_is_active && spring_reset_time == 0)
		spring_reset_time = DEFAULT_SPRING_RESET_TIME;*/
}

bool SpringObject::Init(PhysSimulation *p) {
	simulation = p;
	spring_reset_time = 0;
	spring_is_active = true;
	return BaseInit();
}

SpringObject::SpringObject() {}
SpringObject::~SpringObject() {}

void SpringObject::Collide(Object* obj) {
	if (obj->GetProperties().is_player) {
		
		// Spring it!
		if (spring_reset_time == 0) {
			currentAnimation->Unfreeze();
			spring_reset_time = DEFAULT_SPRING_RESET_TIME;
		}
	}
}


