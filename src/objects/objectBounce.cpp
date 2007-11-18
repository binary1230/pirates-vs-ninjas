#include "stdafx.h"
#include "objectBounce.h"

#include "globals.h"
#include "animation.h"
#include "gameState.h"
#include "gameWorld.h"
#include "gameSound.h"

#define MIN_VELOCITY 0.34f
#define FRICTION_MULTIPLIER 0.70f

void ObjectBounce::Shutdown() {
	BaseShutdown();
}

void ObjectBounce::Update() {
	BaseUpdate();
	UpdateSimpleAnimations();

	if (play_hit_sound) {
		// SOUND->PlaySound("ball_hit");
		play_hit_sound = false;
	}
	
	collided_last_frame = d.down;

	// ghettoooooooo friction.
	if (d.down) {
		vel.SetX(vel.GetX() * FRICTION_MULTIPLIER);
		if (fabs(vel.GetX()) < MIN_VELOCITY) {
			vel.SetX(0);
		}
	}
}

bool ObjectBounce::Init() {
	play_hit_sound = false;
	collided_last_frame = false;
	return BaseInit();
}

void ObjectBounce::Collide(Object* obj) {
	if (!properties.is_solid || obj->GetProperties().is_ball || obj->GetProperties().is_fan)
		return;

	if (obj->GetProperties().is_solid && !obj->GetProperties().is_player) {
    Vector2D newpos;
    d = GetBound(obj, newpos);

    pos = newpos;
    UpdateProjectionRectFromCollisions(newpos);

    if (d.left || d.right) {
      vel.SetX(-vel.GetX());
		}

    if (d.down || d.up) {
      vel.SetY( -vel.GetY()*0.61f );
			if (!collided_last_frame)
				play_hit_sound = true;
			collided_last_frame = true;
		}
  }
}

ObjectBounce::ObjectBounce() {}
ObjectBounce::~ObjectBounce() {}
