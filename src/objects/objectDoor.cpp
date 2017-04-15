#include "stdafx.h"
#include "objectDoor.h"

#include "globals.h"
#include "animation.h"
#include "gameState.h"
#include "gameWorld.h"
#include "gameSound.h"
#include "animations.h"

void ObjectDoor::Shutdown() {
	BaseShutdown();
}

// in seconds.
#define TIME_TO_WAIT_BEFORE_DOOR_ACTION 30

// activate the door.
void ObjectDoor::Activate() {
	door_open_time = 0;
	WORLD->SetModalObject(this);

	PlayAnimation(DOOR_OPENING);
}

void ObjectDoor::DoDoorAction() {
	
	// this door now no longer grabs exlusive control of the level.	
	WORLD->SetModalObject(NULL);
			
	GameModeExitInfo exitInfo, oldExitInfo;
	exitInfo = WORLD->GetExitInfo();
	oldExitInfo = WORLD->GetOldExitInfo();
	
	// figure out what to do based on the door type
	switch (door_type) {

		case LEVEL_EXIT:
			exitInfo.showInitialText = true;
			GAMESTATE->SignalEndCurrentMode();
			break;
		
		case SWITCH_TO_ANOTHER_MODE: 

			if (door_name.length() > 0) {
				exitInfo.useLastPortalName = true;
				exitInfo.lastPortalName = door_name;
			}

			if (mode_to_jump_to_on_activate.length() > 0) {
				exitInfo.useNextModeToLoad = true;
				exitInfo.nextModeToLoad = mode_to_jump_to_on_activate;
			}

			exitInfo.showInitialText = false;

			WORLD->SetExitInfo(exitInfo);
			GAMESTATE->SignalEndCurrentMode();
			break;

		case RETURN_TO_LAST_MODE:

			if (oldExitInfo.useExitInfo && oldExitInfo.useLastPortalName) {
				exitInfo.useLastPortalName = true;
				exitInfo.lastPortalName = oldExitInfo.lastPortalName;
			}

			if (oldExitInfo.useExitInfo && oldExitInfo.lastModeName.length() > 0) {
				exitInfo.useNextModeToLoad = true;
				exitInfo.nextModeToLoad = oldExitInfo.lastModeName;
			}
			
			exitInfo.showInitialText = false;
	
			WORLD->SetExitInfo(exitInfo);
			GAMESTATE->SignalEndCurrentMode();
			break;

		case WARP_TO_ANOTHER_PORTAL:
			assert(0 && "TODO: Warping not supported yet, sorry.");
			break;

		default: case INVALID_TYPE:
			assert(0 && "Invalid Door type triggered!");
			break;
	}
}

void ObjectDoor::Update() {
	BaseUpdate();
	UpdateSimpleAnimations();
	
	if (door_open_time == -1)
		return;

	++door_open_time;

	// yea, hackish.  really should have some way to know when the animation
	// was over.
	if (door_open_time >= TIME_TO_WAIT_BEFORE_DOOR_ACTION)
		DoDoorAction();
}

bool ObjectDoor::Init() {
	door_open_time = -1;
	return BaseInit();
}

void ObjectDoor::Clear() {
	m_animationMapping = GetDoorAnimationMappings();
}

ObjectDoor::ObjectDoor() {
	Clear();
}
ObjectDoor::~ObjectDoor() {}

BOOST_CLASS_EXPORT_GUID(ObjectDoor, "ObjectDoor")