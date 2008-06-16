#include "stdafx.h"
#include "objectIDs.h"
#include "animations.h"

// y'know - this file is probably stupid.

//! This file has hardcoded animation data that I do not think
//! will be practical to abstract further.
//! 
//! Each of these functions returns a mapping of string->int's which 
//! is the mapping of animation NAMES to ID's.  

AnimationMapping GetPlayerAnimationMappings() {
	AnimationMapping animation;
	animation["walking"] = PLAYER_WALKING;
	animation["standing"] = PLAYER_STANDING;
	animation["sliding"] = PLAYER_SLIDING_DOWN_WALL;
	animation["jumping"] = PLAYER_JUMPING;
	animation["lookup"] = PLAYER_LOOKUP;
	return animation;
}

AnimationMapping GetDoorAnimationMappings() {
	AnimationMapping animation;
	animation["closed"] = DOOR_CLOSED;
	animation["opening"] = DOOR_OPENING;
	return animation;
}
