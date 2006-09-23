#include "objectDoor.h"

#include "globals.h"
#include "animation.h"
#include "gameState.h"

// activate the door's action
// someday this will be a 'switch (action) {case warp: ... }' etc
void DoorObject::Activate() {

	// XXX perhaps fade, or SOMETHING here instead of just exiting.
	
	GAMESTATE->SignalEndCurrentMode();
}

void DoorObject::Update() {
	BaseUpdate();
	UpdateSimpleAnimations();
}

bool DoorObject::Init(PhysSimulation *p) {
	simulation = p;
	return BaseInit();
}

DoorObject::DoorObject() {}
DoorObject::~DoorObject() {}
