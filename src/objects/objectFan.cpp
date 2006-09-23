#include "objectFan.h"

#include "globals.h"
#include "animation.h"
#include "gameState.h"

// all of this class is slightly hackish for the moment.
#define FAN_DECAY_RATE 0.98f
#define MAX_SPEED 5
#define DEFAULT_FAN_SPEED 5.0f
#define MAX_PLAYER_SPEED 10.0f

void FanObject::Update() {

	int anim_speed;

	BaseUpdate();
	
	current_fan_speed *= FAN_DECAY_RATE;

	anim_speed = max(MAX_SPEED - int(current_fan_speed), 1);

	if (currentAnimation) {
		if (anim_speed == MAX_SPEED)
			currentAnimation->Freeze();
		else
			currentAnimation->Unfreeze();

		currentAnimation->SetSpeedMultiplier(anim_speed);
		currentAnimation->Update();
		currentSprite = currentAnimation->GetCurrentSprite();
	}
}

void FanObject::Collide(Object* obj) {
	if (obj->GetProperties().is_player) {

		// if the player is going slowly, slow down the new fan speed
		float player_factor = fmin(MAX_PLAYER_SPEED, fabs(obj->GetVelX())) 
													/ MAX_PLAYER_SPEED;
		
		current_fan_speed = 
		 max(DEFAULT_FAN_SPEED * player_factor, current_fan_speed);
	}
}

bool FanObject::Init(PhysSimulation *p) {
	current_fan_speed = 0.0f;

	simulation = p;
	return BaseInit();
}

FanObject::FanObject() {}
FanObject::~FanObject() {}
