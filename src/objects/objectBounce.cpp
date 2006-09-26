#include "objectBounce.h"

#include "globals.h"
#include "animation.h"
#include "gameState.h"
#include "physSimulation.h"

void BounceObject::Update() {
	BaseUpdate();
	UpdateSimpleAnimations();

	// ghettoooooooo friction.
	if (d.down)
		vel.SetX(vel.GetX() * 0.90f);
}

bool BounceObject::Init(PhysSimulation *p) {
	simulation = p;
	return BaseInit();
}

void BounceObject::Collide(Object* obj) {
	if (!properties.is_solid || obj->GetProperties().is_ball)
		return;

	if (obj->GetProperties().is_solid) {
    Vector2D newpos;
    d = GetBound(obj, newpos);

    pos = newpos;
    UpdateProjectionRectFromCollisions(newpos);

    if (d.left || d.right)
      vel.SetX(-vel.GetX());

    if (d.down || d.up)
      vel.SetY( -vel.GetY()*0.83f );
  }
}

BounceObject::BounceObject() {}
BounceObject::~BounceObject() {}
