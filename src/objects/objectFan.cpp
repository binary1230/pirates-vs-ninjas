#include "objectFan.h"

#include "globals.h"
#include "animation.h"
#include "gameState.h"

// all of this class is slightly hackish for the moment.
#define FAN_DECAY_RATE 0.96f
#define MAX_PLAYER_SPEED 10.0f
#define DEFAULT_FAN_VELOCITY 30.0f

void FanObject::Shutdown() {
	BaseShutdown();
}

void FanObject::Update() {
	use_rotation = true;
	rotate_velocity *= FAN_DECAY_RATE;
	
	BaseUpdate();
	UpdateSimpleAnimations();
}

void FanObject::Collide(Object* obj) {
	if (obj->GetProperties().is_player) {

		// if the player is going slowly, slow down the new fan speed
		float player_factor = fmin(	MAX_PLAYER_SPEED, fabs(obj->GetVelX()) ); 
		player_factor /= MAX_PLAYER_SPEED;

		// figure out which way to spin the fan (left/right)
		float direction = -1.0f;
		if (obj->GetVelX() < 0)
			direction = 1.0f;

		// calc the new rotational velocity
		rotate_velocity = direction * 
						fmax(DEFAULT_FAN_VELOCITY*player_factor, fabs(rotate_velocity));
	}
}

bool FanObject::Init() {
	return BaseInit();
}

FanObject::FanObject() {}
FanObject::~FanObject() {}
