#include "objectSpring.h"

#include "globals.h"
#include "animation.h"
#include "gameState.h"
#include "gameSound.h"

#define DEFAULT_TIME_TO_WAIT_BEFORE_SOUND 10

void SpringObject::Shutdown() {
	BaseShutdown();
}

void SpringObject::Update() {
	BaseUpdate();
	UpdateSimpleAnimations();

	if (time_to_wait_before_sound > 0)
		time_to_wait_before_sound--;
}

bool SpringObject::Init(PhysSimulation *p) {
	simulation = p;
	time_to_wait_before_sound = 0;
	return BaseInit();
}

SpringObject::SpringObject() {}
SpringObject::~SpringObject() {}

void SpringObject::Collide(Object* obj) {
	if (obj->GetProperties().is_player) {
		currentAnimation->Unfreeze();
		if (time_to_wait_before_sound == 0) {
			SOUND->PlaySound("spring");
			time_to_wait_before_sound = DEFAULT_TIME_TO_WAIT_BEFORE_SOUND;
		}
	}
}


