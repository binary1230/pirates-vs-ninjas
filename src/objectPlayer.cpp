#include "objectPlayer.h"

#include "gameState.h"
#include "globals.h"
#include "input.h"
#include "gameSound.h"
#include "force.h"
#include "forceGravity.h"
#include "forceInput.h"
#include "vector2D.h"
#include "animation.h"
#include "animations.h"
#include "physSimulation.h"

#define DEFAULT_JUMP_VELOCITY 8.0f
#define DEFAULT_DRAG 0.95f
#define DEFAULT_MIN_VELOCITY 0.3f

void PlayerObject::ScreenBoundsConstraint() {
	if (pos.GetX() < 0) {
		// vel.SetX(-vel.GetX()); // flip velocity
		vel.SetX(0); 							// stop
		pos.SetX(0);
	} else if (pos.GetX() > (simulation->GetWidth() - GetWidth()) ) {
		// vel.SetX(-vel.GetX());	// flip velocity
		vel.SetX(0);							// stop
		pos.SetX(simulation->GetWidth() - GetWidth());
	}
}

void PlayerObject::UpdateSpriteFlip() {
	if (accel.GetX() == 0.0f) {
		if (vel.GetX() > 0.0f)
			flip_x = false;
		else if (vel.GetX() < 0.0f)
			flip_x = true;
	} else if (accel.GetX() > 0.0f) {
		flip_x = false;
	} else {
		flip_x = true;
	}
}

void PlayerObject::UpdateRunningAnimationSpeed() {
	// alter the speed of the animation based on the velocity
	// fprintf(stderr, "vel=%f\n", fabs(vel.GetX()));
	if (fabs(vel.GetX()) < 3.0f)
		currentAnimation->SetSpeedMultiplier(10);// slow
	else if (fabs(vel.GetX()) < 7.0f)
		currentAnimation->SetSpeedMultiplier(6);// med
	else if (fabs(vel.GetX()) < 13.0f)
		currentAnimation->SetSpeedMultiplier(2);// slight fast
	else 
		currentAnimation->SetSpeedMultiplier(1);// max
}

// Things common to STANDING, WALKING, and RUNNING
void PlayerObject::DoCommonGroundStuff() {
	if (!d.down) {
		DoFalling();
		return;
	}	

	if (input->KeyOnce(PLAYERKEY_JUMP, controller_num)) {
		vel.SetY(jump_velocity);
		PlaySound("jump");
		DoJumping();
		return;
  }	
	
	// Do ghetto friction
	vel *= drag;
}

void PlayerObject::DoStanding() {	
	state = STANDING;

	DoCommonGroundStuff();

	currentAnimation = animations[PLAYER_STANDING];

	if (fabs(accel.GetX()) > 0.0f || fabs(vel.GetX()) > 0.0f ) {
		DoWalking();
		return;
	}
}

void PlayerObject::DoWalking() {
	state = WALKING;

	DoCommonGroundStuff();
	
	currentAnimation = animations[PLAYER_WALKING];

	if (accel.GetX() == 0.0f && fabs(vel.GetX()) < min_velocity) {
		vel.SetX(0);
		DoStanding();
	}
		
	UpdateRunningAnimationSpeed();
}

// no distinction from walking yet.
void PlayerObject::DoRunning() {
	state = RUNNING;
}

void PlayerObject::DoJumping() {
	state = JUMPING;

	currentAnimation = animations[PLAYER_JUMPING];

	// really shouldn't have a downward 
	// collision on an upward jump
	if (d.down) {
		DoStanding();
		return;
	}

	if (vel.GetY() < 0) {
		DoFalling();
		return;
	}
}

void PlayerObject::DoFalling() {
	state = FALLING;

	currentAnimation = animations[PLAYER_JUMPING];

	if (d.down) {
		DoStanding();
	}
}

void PlayerObject::DoWhistling() {
	state = WHISTLING;
}
void PlayerObject::DoLookingUp() {
	state = LOOKINGUP;
}
void PlayerObject::DoCrouchingDown() {
	state = CROUCHINGDOWN;
}

void PlayerObject::Update() {
	UpdateFade();
	
	assert(currentAnimation != NULL);
	currentAnimation->Update();

	ScreenBoundsConstraint();
	UpdateState();
	UpdateSpriteFlip();

	// set the current sprite to the current animation
	currentSprite = currentAnimation->GetCurrentSprite();
}

void PlayerObject::Collide(Object* obj) {
  if (obj->GetProperties().is_solid) {
    
		Vector2D newpos;
    d = GetBound(obj, newpos);
    pos = newpos;

    if (d.left || d.right)
      vel.SetX(0);

    if (d.down) {
      vel.SetY(0);
		}
  }

	if (obj->GetProperties().is_spring) {
		vel.SetY(obj->GetProperties().spring_strength);
	}
}

bool PlayerObject::Init(GameState* _game_state) {
	SetGameState(_game_state);
	input = GetGameState()->GetInput();
	
	controller_num = 1;
	state = FALLING; 

	return BaseInit();
}

PlayerObject::PlayerObject() {
	jump_velocity = DEFAULT_JUMP_VELOCITY;
	min_velocity = DEFAULT_MIN_VELOCITY;
	mass = 1.0f;
	drag = DEFAULT_DRAG;
	state = FALLING;
}


PlayerObject::~PlayerObject() {}

//! Factory method, creates new PlayerObjects from XML files
//
//! NOTE: this only takes an ObjectDefinition XML fragment,
// memory leaks on failures here.. CLEAN IT.
Object* PlayerObject::New(GameState* gameState, XMLNode &xDef, XMLNode &xObj) {
	
	// ObjectProperties props;
	PlayerObject* obj = new PlayerObject();

	// init the object
	if (!obj || !obj->Init(gameState) )
		return NULL;

	// load the animations
	AnimationMapping animation_map = GetPlayerAnimationMappings();
	if (!obj->LoadAnimations(xDef, &animation_map))
		return NULL;

	// load the sounds
	if (!obj->LoadSounds(xDef))
		return NULL;
	
	// get the object properties
	if (!obj->LoadProperties(xDef))
		return NULL;

	if (!obj->LoadPlayerProperties(xDef))
		return NULL;

	return obj;
}

bool PlayerObject::LoadPlayerProperties(XMLNode &xDef) {
	XMLNode xProps = xDef.getChildNode("properties");

	properties.is_player = 1;
	properties.is_solid = 1;

	return (xProps.getChildNode("jumpVelocity").getFloat(jump_velocity) &&
					xProps.getChildNode("minVelocity").getFloat(min_velocity) &&
					xProps.getChildNode("drag").getFloat(drag) );
}

void PlayerObject::UpdateState() {
	switch (state) {
		case STANDING:
			DoStanding();
			break;
		case WALKING:
			DoWalking();
			break;
		case RUNNING:
			DoRunning();
			break;
		case JUMPING:
			DoJumping();
			break;
		case FALLING:
			DoFalling();
			break;
		case WHISTLING:
			DoWhistling();
			break;
		case LOOKINGUP:
			DoLookingUp();
			break;
		case CROUCHINGDOWN:
			DoCrouchingDown();
			break;
		default:
			fprintf(stderr, " -- PLAYEROBJECT ERROR: Unkown state asked for!\n");
			assert(NULL);
			break;
	}
}
