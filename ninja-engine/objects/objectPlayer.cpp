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
#include "objectCollectable.h"

#define DEFAULT_JUMP_VELOCITY 9.0f
#define DEFAULT_DRAG 0.95f
#define DEFAULT_MIN_VELOCITY 0.3f

// The name of the object to spawn for "skidding"
// like the white puffs when you skid
#define SKID_OBJECT_TYPE "skid"

bool ObjectPlayer::WantsToSlideOnLeftSide()
{
	return !m_kCurrentCollision.down && m_kCurrentCollision.left && INPUT->Key(PLAYERKEY_LEFT, controller_num);
}

bool ObjectPlayer::WantsToSlideOnRightSide()
{
	return !m_kCurrentCollision.down && m_kCurrentCollision.right && INPUT->Key(PLAYERKEY_RIGHT, controller_num);
}

bool ObjectPlayer::WantsToSlideOnAnySide()
{
	return (WantsToSlideOnLeftSide() || WantsToSlideOnRightSide());
}

void ObjectPlayer::UpdateSpriteFlip() {
	// might need a little updating.  seems to mostly work ok though.

	if (GetVelX() > 0.0f)
		flip_x = false;
	else if (GetVelX() < 0.0f)
		flip_x = true;
}

void ObjectPlayer::UpdateRunningAnimationSpeed() 
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

void ObjectPlayer::UpdateLeftRightMotion()
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

		assert(_physics_body);
		float impulse = _physics_body->GetMass() * velChange;

		ApplyImpulse(impulse, 0.0f);
	}
}

void ObjectPlayer::Update() 
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
			_physics_body->ApplyLinearImpulseToCenter(
				b2Vec2(0.0f, _physics_body->GetMass() * DEFAULT_JUMP_VELOCITY), true
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
			_physics_body->SetTransform(_physics_body->GetWorldCenter() + b2Vec2(PIXELS_TO_METERS(iHackPixelOffset), 0.0f), _physics_body->GetAngle());

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
		TRACE(" -- PLAYEROBJECT ERROR: Unkown _state asked for!\n");
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

#define CLOSE_TO(n, threshold) (fabs(n) < threshold)

void ObjectPlayer::OnCollide(Object* obj, const b2WorldManifold* pkbWorldManifold)
{
	if (!obj)
		return;

	if (!obj->IsSensor()) {
		if (pkbWorldManifold->normal.y > 0 && CLOSE_TO(pkbWorldManifold->normal.x, 0.1f))
			m_kCurrentCollision.down = 1;

		if (pkbWorldManifold->normal.y < 0 && CLOSE_TO(pkbWorldManifold->normal.x, 0.1f))
			m_kCurrentCollision.up = 1;

		if (pkbWorldManifold->normal.x > 0 && CLOSE_TO(pkbWorldManifold->normal.y, 0.1f))
			m_kCurrentCollision.left = 1;

		if (pkbWorldManifold->normal.x < 0 && CLOSE_TO(pkbWorldManifold->normal.y, 0.1f))
			m_kCurrentCollision.right = 1;
	}

	if (ObjectDoor* door = dynamic_cast<ObjectDoor*>(obj)) {
		door_in_front_of_us = door;
	}

	if (ObjectSpring* spring = dynamic_cast<ObjectSpring*>(obj)) {
		// this should go into the spring class, not here, probably.
		if (spring->IsSpringActive())
		{
			SetVelX(spring->GetPropDirection()->x);
			SetVelY(spring->GetPropDirection()->y);
		}
	}
}

void ObjectPlayer::ResetVolatileState(VolatileStateLevel level) {
	if (level >= LEVEL_PLAYERS) {
		SetXY(_pos_at_load);
	}
}

void ObjectPlayer::DropBombsIfNeeded()
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

	b2Vec2 ball_pos = objBall->GetXY();

	if (GetInput(PLAYERKEY_UP, controller_num)) {
		ball_pos.y += 40;
		objBall->ApplyImpulse(0.0f, strength*0.2f);
	} else if (GetInput(PLAYERKEY_DOWN, controller_num) && !m_kCurrentCollision.down) {
		ball_pos.y -= 40;
		objBall->ApplyImpulse(0.0f, -strength*0.2f);
	} else {
		ball_pos.x += (20 * sign);
		objBall->ApplyImpulse(GetVelX()*0.01f + sign * strength * 0.3f, 0.0f);
	}

	objBall->SetXY(ball_pos);
}

void ObjectPlayer::ScreenBoundsConstraint() {
	if (!WORLD->PlayerAllowedOffscreen()) {
		// TODO: clean this up some.
		// BUG: freaks out on right side of screen

		if (_Pos.x < 0) {
			SetVelX(0.0f);
			int newPosX = 20;
			_physics_body->SetTransform(b2Vec2(PIXELS_TO_METERS(newPosX), _physics_body->GetWorldCenter().y), _physics_body->GetAngle());
			_Pos.x = newPosX;
			UpdatePositionFromPhysicsLocation();
		}
		else if (_Pos.x >(WORLD->GetWidth() - GetWidth())) {
			SetVelX(0.0f);
			float newPosX = WORLD->GetWidth() - GetWidth();
			_physics_body->SetTransform(
				b2Vec2(PIXELS_TO_METERS(newPosX), _physics_body->GetWorldCenter().y), 
				_physics_body->GetAngle()
			);
			UpdatePositionFromPhysicsLocation();
		}
	}
}

void ObjectPlayer::OnAnimationLooped()
{
	m_bShouldNotSwitchAnimationsRightNow = false;
}

void ObjectPlayer::PlayAnimation(uint uiIndex)
{
	if (m_bShouldNotSwitchAnimationsRightNow)
		return;

	Object::PlayAnimation(uiIndex);
}

void ObjectPlayer::Shutdown() {
	BaseShutdown();
}

void ObjectPlayer::Clear()
{
	Object::Clear();

	m_animationMapping = GetPlayerAnimationMappings();

	jump_velocity = DEFAULT_JUMP_VELOCITY;
	min_velocity = DEFAULT_MIN_VELOCITY;
	drag = DEFAULT_DRAG;

	next_skid_time = 0;

	controller_num = 1;
	m_kPlayerState = FALLING;
	door_in_front_of_us = NULL;
	ring_count = 0;
	m_bShouldNotSwitchAnimationsRightNow = false;
}

bool ObjectPlayer::LoadObjectProperties(XMLNode &xDef) {
	if (!Object::LoadObjectProperties(xDef))
		return false;

	uses_physics_engine = 1;
	ignores_physics_rotation = 1;
	use_angled_corners_collision_box = 1;

	XMLNode xProps = xDef.getChildNode("properties");

	_pos_at_load = _Pos;

	return	xProps.getChildNode("jumpVelocity").getFloat(jump_velocity) &&
			xProps.getChildNode("minVelocity").getFloat(min_velocity) &&
			xProps.getChildNode("drag").getFloat(drag);

	_physics_category = PLAYER;
}

bool ObjectPlayer::Init()
{
	return BaseInit();
}

bool ObjectPlayer::GetInput(uint key, uint controller_num) const
{
	return INPUT->Key(key, controller_num);
}

ObjectPlayer::ObjectPlayer() { 
	Clear();
}
ObjectPlayer::~ObjectPlayer() {}

BOOST_CLASS_EXPORT_GUID(ObjectPlayer, "ObjectPlayer")