#include "objectBounce.h"

#include "globals.h"
#include "animation.h"
#include "gameState.h"
#include "physSimulation.h"
#include "gameSound.h"

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
	if (d.down)
		vel.SetX(vel.GetX() * 0.90f);
}

bool ObjectBounce::Init(PhysSimulation *p) {
	simulation = p;
	play_hit_sound = false;
	collided_last_frame = false;
	return BaseInit();
}

void ObjectBounce::Collide(Object* obj) {
	if (!properties.is_solid || obj->GetProperties().is_ball)
		return;

	if (obj->GetProperties().is_solid) {
    Vector2D newpos;
    d = GetBound(obj, newpos);

    pos = newpos;
    UpdateProjectionRectFromCollisions(newpos);

    if (d.left || d.right) {
      vel.SetX(-vel.GetX());
		}

    if (d.down || d.up) {
      vel.SetY( -vel.GetY()*0.83f );
			if (!collided_last_frame)
				play_hit_sound = true;
			collided_last_frame = true;
		}
  }
}

ObjectBounce::ObjectBounce() {}
ObjectBounce::~ObjectBounce() {}
