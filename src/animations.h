#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include "stdafx.h"

#include "animation.h"
#include "objectIDs.h"

//! This file has hardcoded animation data that I do not think
//! will be practical to abstract further.
//! 
//! Each of these functions returns a mapping of string->int's which 
//! is the mapping of animation NAMES to ID's.  

AnimationMapping GetPlayerAnimationMappings();

#endif // ANIMATIONS_H
