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

void PlayerObject::Update() {
	
	assert(currentAnimation != NULL);
	currentAnimation->Update();
				
	int w = simulation->GetWidth();

	// See if we're out of bounds
	if (pos.GetX() < 0) {
		vel.SetX(-vel.GetX());
		pos.SetX(0);
	} else if (pos.GetX() > (w - GetWidth()) ) {
		vel.SetX(-vel.GetX());
		pos.SetX(w - GetWidth());
	}

	if (d.down)
		on_floor = true;
	else
		on_floor = false;

	if (on_floor == true) {
					
		// Then we can jump.
		if (game_state->GetKey(PLAYERKEY_JUMP, controller_num)) {
			vel.SetY(jump_velocity);
			on_floor = false;
			PlaySound("jump");
	  }	else {
			vel *= drag;	
			
			// If on floor, do we draw the standing sprite?
			if (accel.GetX() == 0.0f && fabs(vel.GetX()) < min_velocity) {
				vel.SetX(0);
				currentAnimation = animations[PLAYER_STANDING];
			} else {
				currentAnimation = animations[PLAYER_WALKING];

				// alter the speed of the animation based on the velocity
				// fprintf(stderr, "vel=%f\n", fabs(vel.GetX()));
				if (fabs(vel.GetX()) < 3.0f)
					currentAnimation->SetSpeedMultiplier(5);//slow
				else if (fabs(vel.GetX()) < 7.0f)
					currentAnimation->SetSpeedMultiplier(3);//med
				else 
					currentAnimation->SetSpeedMultiplier(1);//fast
			}
		}
	} else {
		// We're not on the floor, we're falling or jumping
		currentAnimation = animations[PLAYER_JUMPING];
	}

	// figure out whether to flip the sprite or not
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
			on_floor = true;
		}
  }

	if (obj->GetProperties().is_spring) {
		vel.SetY(20);
	}
}

bool PlayerObject::Init(GameState* _game_state) {
	SetGameState(_game_state);
	
	controller_num = 1;
	on_floor = 0;

	return BaseInit();
}

PlayerObject::PlayerObject() {
	jump_velocity = DEFAULT_JUMP_VELOCITY;
	min_velocity = DEFAULT_MIN_VELOCITY;
	mass = 1.0f;
	drag = DEFAULT_DRAG;
	on_floor = 0;
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

	//! Maps a sound to an integer handle (e.g. "jump" to 321, etc)
	typedef map<const CString, uint> SoundMapping;

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

	sscanf(xProps.getChildNode("jumpVelocity").getText(), 
									"%f", &jump_velocity);
	sscanf(xProps.getChildNode("minVelocity").getText(), 
									"%f", &min_velocity);
	sscanf(xProps.getChildNode("drag").getText(), 
									"%f", &drag);

	return true;
}
