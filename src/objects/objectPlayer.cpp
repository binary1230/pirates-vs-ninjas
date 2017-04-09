#include "stdafx.h"
#include "objectPlayer.h"

#include "gameState.h"
#include "globals.h"
#include "input.h"
#include "gameSound.h"
#include "animation.h"
#include "animations.h"
#include "gameWorld.h"
#include "objectDoor.h"
#include "objectFactory.h"
#include "objectSpring.h"
#include "gameSound.h"
#include "effectsManager.h"
#include "globalDefines.h"
#include "physics.h"

#define DEFAULT_JUMP_VELOCITY 9.0f
#define DEFAULT_DRAG 0.95f
#define DEFAULT_MIN_VELOCITY 0.3f

// The name of the object to spawn for "skidding"
// like the white puffs when you skid
#define SKID_OBJECT_TYPE "skid"

bool PlayerObject::WantsToSlideOnLeftSide()
{
	return !m_kCurrentCollision.down && m_kCurrentCollision.left && INPUT->Key(PLAYERKEY_LEFT, controller_num);
}

bool PlayerObject::WantsToSlideOnRightSide()
{
	return !m_kCurrentCollision.down && m_kCurrentCollision.right && INPUT->Key(PLAYERKEY_RIGHT, controller_num);
}

bool PlayerObject::WantsToSlideOnAnySide()
{
	return (WantsToSlideOnLeftSide() || WantsToSlideOnRightSide());
}

void PlayerObject::UpdateSpriteFlip() {
	// might need a little updating.  seems to mostly work ok though.

	if (GetVelX() > 0.0f)
		flip_x = false;
	else if (GetVelX() < 0.0f)
		flip_x = true;
}

void PlayerObject::UpdateRunningAnimationSpeed() 
{
	if (m_bShouldNotSwitchAnimationsRightNow)
		return;

	if (fabs(GetVelX()) < 3.0f)
		currentAnimation->SetSpeedMultiplier(10);	// slow
	else if (fabs(GetVelX()) < 7.0f)
		currentAnimation->SetSpeedMultiplier(6);	// med
	else if (fabs(GetVelX()) < 13.0f)
		currentAnimation->SetSpeedMultiplier(2);	// slight fast
	else 
		currentAnimation->SetSpeedMultiplier(2);	// max
}

void PlayerObject::UpdateLeftRightMotion()
{
	float max_desired_speed = 10.0f;
	float boost = 7.0f;

	const bool go_left = INPUT->Key(PLAYERKEY_LEFT, controller_num) && !INPUT->Key(PLAYERKEY_RIGHT, controller_num);
	const bool go_right = INPUT->Key(PLAYERKEY_RIGHT, controller_num) && !INPUT->Key(PLAYERKEY_LEFT, controller_num);

	if (go_left ^ go_right) {
		float desiredVelocity;
		if (go_left) {
			desiredVelocity = std::max(GetVelX() - 0.1f, -max_desired_speed);
		}
		else {
			desiredVelocity = std::min(GetVelX() + 0.1f, max_desired_speed);
		}

		float velChange = desiredVelocity - GetVelX();

		// if we're just starting to move and not moving very fast in the direction we want to go, bump up the force.
		// still seem to be slowing a little on friction when coming down from landing.
		float percent_max_speed = 1.0f - (max_desired_speed - fabs(velChange)) / max_desired_speed;
		if (percent_max_speed < 0.5f) {
			velChange *= boost;
		}

		float impulse = m_pkPhysicsBody->GetMass() * velChange;

		ApplyImpulse(impulse, 0.0f);
	}
}

void PlayerObject::Update() 
{
	BaseUpdate();

	UpdateLeftRightMotion();
	
	if (currentAnimation)
		currentAnimation->Update();

	DropBombsIfNeeded();

	bool bNoLeft;
	bool bNoRight;

	switch (m_kPlayerState) {
	case STANDING: // or walking.
		if (!m_kCurrentCollision.down) {
			m_kPlayerState = FALLING;
			break;
		}

		if (door_in_front_of_us && INPUT->KeyOnce(PLAYERKEY_UP, controller_num))
		{
			m_kPlayerState = WALKING_THRU_DOOR;
			break;
		}

		if (INPUT->KeyOnce(PLAYERKEY_JUMP, controller_num))
		{
			m_pkPhysicsBody->ApplyLinearImpulseToCenter(
				b2Vec2(0.0f, m_pkPhysicsBody->GetMass() * DEFAULT_JUMP_VELOCITY), true
			);
			SOUND->PlaySound("jump");
			m_kPlayerState = JUMPING;
			break;
		}

		if (fabs(GetVelX()) <= 0.0f) {
			PlayAnimation(PLAYER_STANDING);
		} else {
			PlayAnimation(PLAYER_WALKING);
			UpdateRunningAnimationSpeed();
		}

		break;
	case JUMPING:
		PlayAnimation(PLAYER_JUMPING);

		if (WantsToSlideOnAnySide())
		{
			m_kPlayerState = SLIDING_DOWN_WALL;
			break;
		}

		// really shouldn't have a downward 
		// collision on an upward jump but....
		if (m_kCurrentCollision.down) {
			m_kPlayerState = STANDING;
			break;
		}

		if (GetVelY() < 0) {
			m_kPlayerState = FALLING;
			break;
		}
		break;
	case FALLING:
		PlayAnimation(PLAYER_JUMPING);

		if (WantsToSlideOnAnySide()) {
			m_kPlayerState = SLIDING_DOWN_WALL;
			break;
		}

		if (m_kCurrentCollision.down) {
			m_kPlayerState = STANDING;
			break;
		}
	
		break;
	case WALKING_THRU_DOOR:
		PlayAnimation(PLAYER_WALKING);

		if (door_in_front_of_us)
			door_in_front_of_us->Activate();

		break;
	case SLIDING_DOWN_WALL:
		PlayAnimation(PLAYER_SLIDING_DOWN_WALL);

		if (m_kCurrentCollision.down)
		{
			m_kPlayerState = STANDING;
			break;
		}

		if (!m_kCurrentCollision.left && !m_kCurrentCollision.right)
		{
			m_kPlayerState = FALLING;
			break;
		}

		bNoLeft = !m_kCurrentCollision.left || !INPUT->Key(PLAYERKEY_LEFT, controller_num);
		bNoRight = !m_kCurrentCollision.right || !INPUT->Key(PLAYERKEY_RIGHT, controller_num);

		if (bNoLeft && bNoRight)
		{
			m_kPlayerState = FALLING;
			break;
		}

		if (INPUT->KeyOnce(PLAYERKEY_JUMP, controller_num))
		{
			// HACK: offset just the tiniest amount to make us not collide with the wall anymore.
			const int iOffset = 3;
			int iHackPixelOffset = m_kCurrentCollision.left ? iOffset : -iOffset;
			m_pkPhysicsBody->SetTransform(m_pkPhysicsBody->GetWorldCenter() + b2Vec2(PIXELS_TO_METERS(iHackPixelOffset), 0.0f), m_pkPhysicsBody->GetAngle());

			SetVelY(DEFAULT_JUMP_VELOCITY * 0.9f);

			const float fHorizontalJumpVel = DEFAULT_JUMP_VELOCITY * 0.8f;

			if (m_kCurrentCollision.left)
				SetVelX(fHorizontalJumpVel);
			else
				SetVelX(-fHorizontalJumpVel);

			SOUND->PlaySound("jump");

			m_kPlayerState = JUMPING;
			break;
		}
		break;
	default:
		TRACE(" -- PLAYEROBJECT ERROR: Unkown state asked for!\n");
		assert(false);
		break;
	}

	UpdateSpriteFlip();

	// set the current sprite to the current animation
	if (currentAnimation)
		currentSprite = currentAnimation->GetCurrentSprite();
	
	// this will be set at the start of each frame if we're colliding.
	door_in_front_of_us = NULL;	

	ScreenBoundsConstraint();
}

void PlayerObject::OnCollide(Object* obj, const b2WorldManifold* pkbWorldManifold)
{
	if (obj->GetProperties().is_static && obj->GetProperties().is_physical && !obj->GetProperties().is_sensor)
	{
		if (pkbWorldManifold->normal.y > 0 && pkbWorldManifold->normal.x == 0.0f)
			m_kCurrentCollision.down = 1;

		if (pkbWorldManifold->normal.y < 0 && pkbWorldManifold->normal.x == 0.0f)
			m_kCurrentCollision.up = 1;

		if (pkbWorldManifold->normal.x > 0 && pkbWorldManifold->normal.y == 0.0f)
			m_kCurrentCollision.left = 1;

		if (pkbWorldManifold->normal.x < 0 && pkbWorldManifold->normal.y == 0.0f)
			m_kCurrentCollision.right = 1;
	}


	if (obj->GetProperties().is_door) {
		door_in_front_of_us = (DoorObject*)obj;
	}

	if (obj->GetProperties().is_spring)
	{
		SpringObject* sObj = (SpringObject*)obj;

		// this should go into the spring class, not here
		if (sObj->IsSpringActive())
		{
			SetVelX(sObj->GetSpringVector().x);
			SetVelY(sObj->GetSpringVector().y);
		}
	}

	if (obj->GetProperties().is_ring)
		++ring_count;
}

void PlayerObject::DropBombsIfNeeded()
{
	if (m_kPlayerState == WALKING_THRU_DOOR)
		return;

	bool attack = false;
	if (INPUT->KeyOnce(PLAYERKEY_ACTION1, controller_num))
		attack = true;

	if (!attack)
		return;

	Object* objBall = EFFECTS->TriggerEffect(this, "bomb");
	if (!objBall)
		return;

	float sign = flip_x ? -1 : 1;
	float strength = 0.8f;

	if (GetInput(PLAYERKEY_UP, controller_num)) {
		objBall->SetY(objBall->GetY() + 40);
		objBall->ApplyImpulse(0.0f, strength*0.2f);
	} else if (GetInput(PLAYERKEY_DOWN, controller_num) && !m_kCurrentCollision.down) {
		objBall->SetY(objBall->GetY() - 40);
		objBall->ApplyImpulse(0.0f, -strength*0.2f);
	} else {
		objBall->SetX(objBall->GetX() + (20 * sign));
		objBall->ApplyImpulse(GetVelX()*0.01f + sign * strength * 0.3f, 0.0f);
	}
}

void PlayerObject::ScreenBoundsConstraint() {
	if (!WORLD->PlayerAllowedOffscreen()) {
		// TODO: clean this up some.
		// BUG: freaks out on right side of screen

		if (pos.x < 0) {
			SetVelX(0.0f);
			int newPosX = 20;
			m_pkPhysicsBody->SetTransform(b2Vec2(PIXELS_TO_METERS(newPosX), m_pkPhysicsBody->GetWorldCenter().y), m_pkPhysicsBody->GetAngle());
			pos.x = newPosX;
			UpdatePositionFromPhysicsLocation();
		}
		else if (pos.x >(WORLD->GetWidth() - width)) {
			SetVelX(0.0f);
			float newPosX = WORLD->GetWidth() - width;
			m_pkPhysicsBody->SetTransform(
				b2Vec2(PIXELS_TO_METERS(newPosX), m_pkPhysicsBody->GetWorldCenter().y), 
				m_pkPhysicsBody->GetAngle()
			);
			UpdatePositionFromPhysicsLocation();
		}
	}
}

void PlayerObject::OnAnimationLooped()
{
	m_bShouldNotSwitchAnimationsRightNow = false;
}

void PlayerObject::PlayAnimation(uint uiIndex)
{
	if (m_bShouldNotSwitchAnimationsRightNow)
		return;

	Object::PlayAnimation(uiIndex);
}

void PlayerObject::Shutdown() {
	BaseShutdown();
}

bool PlayerObject::Init()
{
	jump_velocity = DEFAULT_JUMP_VELOCITY;
	min_velocity = DEFAULT_MIN_VELOCITY;
	drag = DEFAULT_DRAG;

	next_skid_time = 0;

	controller_num = 1;
	m_kPlayerState = FALLING;
	door_in_front_of_us = NULL;
	ring_count = 0;
	m_bShouldNotSwitchAnimationsRightNow = false;

	return BaseInit();
}

bool PlayerObject::LoadPlayerProperties(XMLNode &xDef) {
	XMLNode xProps = xDef.getChildNode("properties");

	properties.is_player = 1;
	properties.is_physical = 1;
	properties.ignores_physics_rotation = 1;
	properties.use_angled_corners_collision_box = 1;

	on_skateboard = false;

	if (xProps.nChildNode("onSkateboard"))
		on_skateboard = true;

	return (xProps.getChildNode("jumpVelocity").getFloat(jump_velocity) &&
		xProps.getChildNode("minVelocity").getFloat(min_velocity) &&
		xProps.getChildNode("drag").getFloat(drag));
}


PlayerObject::PlayerObject() {}
PlayerObject::~PlayerObject() {}