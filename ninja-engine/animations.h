#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include "animation.h"

//! This file has hardcoded animation data that I do not think
//! will be practical to abstract further.
//! 
//! Each of these functions returns a mapping of string->int's which 
//! is the mapping of animation NAMES to ID's.  

#define PLAYER_WALKING 0
#define PLAYER_STANDING 1
#define PLAYER_JUMPING 2
#define PLAYER_LOOKUP 3
#define PLAYER_SLIDING_DOWN_WALL 4
#define PLAYER_ATTACK1 5 
#define PLAYER_ATTACK2 6
#define PLAYER_ATTACK3 7

#define DOOR_CLOSED 0
#define DOOR_OPENING 1

AnimationMapping GetPlayerAnimationMappings();
AnimationMapping GetDoorAnimationMappings();

#endif // ANIMATIONS_H
