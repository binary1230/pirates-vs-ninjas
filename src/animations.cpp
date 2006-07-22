#include "objectIDs.h"
#include "animations.h"

//! This file has hardcoded animation data that I do not think
//! will be practical to abstract further.
//! 
//! Each of these functions returns a mapping of string->int's which 
//! is the mapping of animation NAMES to ID's.  

// XXX silly. should just use string->ID mapping in the main thing
AnimationMapping GetPlayerAnimationMappings() {
	AnimationMapping animation;
	animation["walking"] = PLAYER_WALKING;
	animation["standing"] = PLAYER_STANDING;
	animation["jumping"] = PLAYER_JUMPING;
	animation["lookup"] = PLAYER_LOOKUP;
	return animation;
}
