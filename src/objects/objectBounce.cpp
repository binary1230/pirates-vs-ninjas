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
	
	collided_last_frame = m_kCurrentCollision.down;

	// ghettoooooooo friction.
	if (m_kCurrentCollision.down) {
		vel.x *= FRICTION_MULTIPLIER;
		if (fabs(vel.x) < MIN_VELOCITY) {
			vel.x = 0;
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
		m_kCurrentCollision = GetBound(obj, newpos);

		pos = newpos;
		UpdateProjectionRectFromCollisions(newpos);

		if (m_kCurrentCollision.left || m_kCurrentCollision.right)
			vel.x = -vel.x;
		
		if (m_kCurrentCollision.down || m_kCurrentCollision.up) {
			vel.y = -vel.y*0.61f;

			if (!collided_last_frame)
				play_hit_sound = true;

			collided_last_frame = true;
		}
	}
}

ObjectBounce::ObjectBounce() {}
ObjectBounce::~ObjectBounce() {}
