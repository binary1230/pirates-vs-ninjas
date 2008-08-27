#include "stdafx.h"
#include "objectPlayer.h"

#include "gameState.h"
#include "globals.h"
#include "input.h"
#include "gameSound.h"
#include "vector2D.h"
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

#define DEFAULT_JUMP_VELOCITY 10.0f
#define DEFAULT_DRAG 0.95f
#define DEFAULT_MIN_VELOCITY 0.3f

// The name of the object to spawn for "skidding"
// like the white puffs when you skid
#define SKID_OBJECT_TYPE "skid"

void PlayerObject::Shutdown() {
	BaseShutdown();
}

void PlayerObject::ScreenBoundsConstraint() {
	if (WORLD->PlayerAllowedOffscreen())
		return;

	// MAN. Abstract away the physics vs our representation here. Think about this more. What a mess.

	if (pos.x < 0) {
		SetVelX(0.0f);
		int newPosX = 2;
		pos.x = newPosX;
		m_pkPhysicsBody->SetXForm(b2Vec2(PIXELS_TO_METERS(newPosX), m_pkPhysicsBody->GetWorldCenter().y), m_pkPhysicsBody->GetAngle());
	} else if (pos.x > (WORLD->GetWidth() - width) ) {
		SetVelX(0.0f);
		int newPosX = WORLD->GetWidth() - width - 1;
		m_pkPhysicsBody->SetXForm(
			b2Vec2(PIXELS_TO_METERS(newPosX), m_pkPhysicsBody->GetWorldCenter().y), m_pkPhysicsBody->GetAngle()
		);
		pos.x = newPosX;
	}
}

void PlayerObject::UpdateSpriteFlip() {

	if (accel.x == 0.0f) 
	{
		if (GetVelX() > 0.0f)
			flip_x = false;
		else if (GetVelX() < 0.0f)
			flip_x = true;
	} 
	else if (accel.x > 0.0f) 
	{
		flip_x = false;
	} 
	else 
	{
		flip_x = true;
	}
}

void PlayerObject::UpdateRunningAnimationSpeed() {
	// alter the speed of the animation based on the velocity
	// TRACE("vel=%f\n", fabs(vel.x));
	if (fabs(GetVelX()) < 3.0f)
		currentAnimation->SetSpeedMultiplier(10);// slow
	else if (fabs(GetVelX()) < 7.0f)
		currentAnimation->SetSpeedMultiplier(6);// med
	else if (fabs(GetVelX()) < 13.0f)
		currentAnimation->SetSpeedMultiplier(2);// slight fast
	else 
		currentAnimation->SetSpeedMultiplier(1);// max
}

void PlayerObject::DoWalkThroughDoor() {
	m_kPlayerState = WALKING_THRU_DOOR;

	// XXX don't have an animation for this yet...
	PlayAnimation(PLAYER_WALKING);

	if (door_in_front_of_us)
		door_in_front_of_us->Activate();

	door_in_front_of_us = NULL;
}

// Things common to STANDING, WALKING, and RUNNING
void PlayerObject::DoCommonGroundStuff() 
{
	if (!m_kCurrentCollision.down) {
		DoFalling();
		return;
	}	

	if (door_in_front_of_us && INPUT->KeyOnce(PLAYERKEY_UP, controller_num)) 
	{
		DoWalkThroughDoor();
		return;
	}

	if (INPUT->KeyOnce(PLAYERKEY_JUMP, controller_num)) 
	{
		SetVelY(DEFAULT_JUMP_VELOCITY);
		SOUND->PlaySound("jump");
		DoJumping();
		return;
	}	
}

void PlayerObject::DoSlidingDownWall()
{
	m_kPlayerState = SLIDING_DOWN_WALL;

	PlayAnimation(PLAYER_SLIDING_DOWN_WALL);

	if (m_kCurrentCollision.down)
	{
		DoStanding();
		return;
	}

	if (!m_kCurrentCollision.left && !m_kCurrentCollision.right)
	{
		DoFalling();
		return;
	}

	bool bNoLeft = !m_kCurrentCollision.left || !INPUT->Key(PLAYERKEY_LEFT, controller_num);
	bool bNoRight = !m_kCurrentCollision.right || !INPUT->Key(PLAYERKEY_RIGHT, controller_num);

	if (bNoLeft && bNoRight)
	{
		DoFalling();
		return;
	}

	if (INPUT->KeyOnce(PLAYERKEY_JUMP, controller_num)) 
	{
		// HACK: offset just the tiniest amount to make us not collide with the wall anymore.
		const int iOffset = 3;
		int iHackPixelOffset = m_kCurrentCollision.left ? iOffset : -iOffset;
		m_pkPhysicsBody->SetXForm(m_pkPhysicsBody->GetWorldCenter() + b2Vec2(PIXELS_TO_METERS(iHackPixelOffset), 0.0f), m_pkPhysicsBody->GetAngle());

		// don't apply any forces
		accel.x = 0.0f;

		SetVelY(DEFAULT_JUMP_VELOCITY);

		const float fHorizontalJumpVel = DEFAULT_JUMP_VELOCITY * 2.0f;
		
		if (m_kCurrentCollision.left)
			SetVelX(fHorizontalJumpVel);
		else
			SetVelX(-fHorizontalJumpVel);

		SOUND->PlaySound("jump");

		m_kPlayerState = JUMPING;
		return;
	}

	LimitMaxVerticalVelocityTo(3.0f);
}

void PlayerObject::DoStanding() 
{	
	m_kPlayerState = STANDING;

	DoCommonGroundStuff();

	PlayAnimation(PLAYER_STANDING);

	if (fabs(accel.x) > 0.0f || fabs(GetVelX()) > 0.0f ) 
	{
		DoWalking();
		return;
	}
}

void PlayerObject::DoWalking() 
{
	m_kPlayerState = WALKING;
	
	DoCommonGroundStuff();
	
	PlayAnimation(PLAYER_WALKING);

	// if we go too slow, then stop us and make us STANDING
	if (accel.x == 0.0f && fabs(GetVelX()) < min_velocity) 
	{
		SetVelX(0);
		DoStanding();
	}

	UpdateSkidding();

	UpdateRunningAnimationSpeed();
}

void PlayerObject::UpdateSkidding() {
	if (next_skid_time > 0)
		next_skid_time--;

	// If acceleration and velocity are in the opposite directions,
	// then we are skidding and trying to turn around
	if (	on_skateboard || 
			(accel.x > 0.0f && GetVelX() < 0.0f) ||
			(accel.x < 0.0f && GetVelX() > 0.0f) ) {

		if (next_skid_time == 0) {
			next_skid_time = 0;

			// Create a "skid" object (little white whisp at player's feet)
			Object* objSkid = EFFECTS->TriggerObject(this, "skid");
			
			if (objSkid) {
				float skid_vel_x = 6.0f;

				if (GetVelX() < 0.0f)
					skid_vel_x *= -1.0f;

				objSkid->SetDisplayTime(Rand(1,10));
				objSkid->SetVelXY(skid_vel_x, 0.0f);
				objSkid->FadeOut(Rand(4,10));
			}
		}
	}
}

bool PlayerObject::WantsToSlideOnLeftSide()
{
	return !m_kCurrentCollision.down && m_kCurrentCollision.left && INPUT->Key(PLAYERKEY_LEFT, controller_num);
}

bool PlayerObject::WantsToSlideOnRightSide()
{
	return !m_kCurrentCollision.down && m_kCurrentCollision.right && INPUT->Key(PLAYERKEY_RIGHT, controller_num);
}

bool PlayerObject::DoCommonAirStuff() 
{
	// If we're not trying to moving at all, slow us down just a bit so we fall DOWN more, not not forward
	if (accel.x > -0.001f && accel.x < 0.001f)
		SetVelX(GetVelX() * 0.90f);

	if (WantsToSlideOnLeftSide() || WantsToSlideOnRightSide())
	{
		DoSlidingDownWall();
		return false;
	}

	return true;
}

// no distinction from walking yet.
void PlayerObject::DoRunning() {
	m_kPlayerState = RUNNING;
}

void PlayerObject::DoJumping() {
	m_kPlayerState = JUMPING;

	PlayAnimation(PLAYER_JUMPING);

	if (!DoCommonAirStuff())
		return;

	// really shouldn't have a downward 
	// collision on an upward jump
	if (m_kCurrentCollision.down) {
		DoStanding();
		return;
	}

	if (GetVelY() < 0) {
		DoFalling();
		return;
	}
}

void PlayerObject::DoFalling() {
	m_kPlayerState = FALLING;

	// XXX: should be PLAYER_FALLING when we have one.
	PlayAnimation(PLAYER_JUMPING);

	const float fMaxFallVelocity = -50.0f;

	if (GetVelY() < fMaxFallVelocity)
		SetVelY(fMaxFallVelocity);

	if (!DoCommonAirStuff())
		return;

	if (m_kCurrentCollision.down) {
		DoStanding();
	}
}

void PlayerObject::DoWhistling() {
	m_kPlayerState = WHISTLING;
}
void PlayerObject::DoLookingUp() {
	m_kPlayerState = LOOKINGUP;
}
void PlayerObject::DoCrouchingDown() {
	m_kPlayerState = CROUCHINGDOWN;
}

// Do things common to most every state
void PlayerObject::DoCommonStuff() 
{	
	DropBombs();
	LimitMaxHorizontalVelocityTo(8.0f);

	// If we're moving in a different direction than what we want to do, make us slow down faster.
	// NOTE: Freaks out horizontal springs currently
	if ((accel.x > 0.001f && GetVelX() < 0.001f) ||
		(accel.x < 0.001f && GetVelX() > 0.001f) ) 
	{
			SetVelX(GetVelX() * 0.85f);
	}
}

void PlayerObject::HandleInput() 
{
	// static float magnitude = GLOBALS->Value("player_acceleration", magnitude);
	static float magnitude = 100;

	// return a force based on 2 inputs.
	if (INPUT->Key(PLAYERKEY_LEFT, controller_num) && 
		!INPUT->Key(PLAYERKEY_RIGHT, controller_num)) 
	{
		accel.x = -magnitude * TIMESTEP;
	}

	else if (INPUT->Key(PLAYERKEY_RIGHT, controller_num) && 
		!INPUT->Key(PLAYERKEY_LEFT, controller_num)) 
	{
		accel.x = magnitude * TIMESTEP;
	}

	// accel *= 700;
}

void PlayerObject::Update() 
{
	BaseUpdate();

	accel.x = 0.0f;
	accel.y = 0.0f;

	HandleInput();
	
	assert(currentAnimation != NULL);
	currentAnimation->Update();

	ScreenBoundsConstraint();
	UpdateState();
	UpdateSpriteFlip();

	// set the current sprite to the current animation
	if (currentAnimation)
		currentSprite = currentAnimation->GetCurrentSprite();
	
	// this will be set true on each collision with a door
	door_in_front_of_us = NULL;

	// apply the acceleration
	m_pkPhysicsBody->ApplyForce(accel, m_pkPhysicsBody->GetWorldCenter());
}

void PlayerObject::OnCollide(Object* obj, const b2ContactPoint* pkContactPoint) 
{
	if (obj->GetProperties().is_door) {
		door_in_front_of_us = (DoorObject*)obj;
		return;
	}
		
	if (obj->GetProperties().is_fan || obj->GetProperties().is_ball)  
		return;

	if (obj->GetProperties().is_static && obj->GetProperties().is_physical)
	{
		if (pkContactPoint->normal.y > 0 && pkContactPoint->normal.x == 0.0f)
			m_kCurrentCollision.down = 1;

		if (pkContactPoint->normal.y < 0 && pkContactPoint->normal.x == 0.0f)
			m_kCurrentCollision.up = 1;

		if (pkContactPoint->normal.x > 0 && pkContactPoint->normal.y == 0.0f)
			m_kCurrentCollision.left = 1;

		if (pkContactPoint->normal.x < 0 && pkContactPoint->normal.y == 0.0f)
			m_kCurrentCollision.right = 1;
	}

	if (obj->GetProperties().is_spring) 
	{
		SpringObject* sObj = (SpringObject*)obj;
		
		if (sObj->IsSpringActive())
			SetVelXY(sObj->GetSpringVector());
	}

	if (obj->GetProperties().is_ring)
		++ring_count;
}

bool PlayerObject::Init() 
{
	next_skid_time = 0;
	
	controller_num = 1;
	m_kPlayerState = FALLING; 
	door_in_front_of_us = NULL;
	ring_count = 0;

	return BaseInit();
}

PlayerObject::PlayerObject() {
	jump_velocity = DEFAULT_JUMP_VELOCITY;
	min_velocity = DEFAULT_MIN_VELOCITY;
	drag = DEFAULT_DRAG;
	m_kPlayerState = FALLING;
	door_in_front_of_us = NULL;
	ring_count = 0;
	next_skid_time = 0;
}

PlayerObject::~PlayerObject() {}

bool PlayerObject::LoadPlayerProperties(XMLNode &xDef) {
	XMLNode xProps = xDef.getChildNode("properties");

	properties.is_player = 1;
	properties.is_physical = 1;
	properties.uses_new_physics = 1;
	properties.ignores_physics_rotation = 1;

	on_skateboard = false;

	if (xProps.nChildNode("onSkateboard"))
		on_skateboard = true;

	return (xProps.getChildNode("jumpVelocity").getFloat(jump_velocity) &&
					xProps.getChildNode("minVelocity").getFloat(min_velocity) &&
					xProps.getChildNode("drag").getFloat(drag) );
}

void PlayerObject::UpdateState() {

	DoCommonStuff();

	switch (m_kPlayerState) {
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
		case WALKING_THRU_DOOR:
			DoWalkThroughDoor();
			break;
		case SLIDING_DOWN_WALL:
			DoSlidingDownWall();
			break;
		default:
			TRACE(" -- PLAYEROBJECT ERROR: Unkown state asked for!\n");
			assert(false);
			break;
	}
}

void PlayerObject::DropBombs()
{
	if (INPUT->KeyOnce(PLAYERKEY_ACTION1, controller_num) && 
		m_kPlayerState != WALKING_THRU_DOOR) 
	{	
		Object* objBall = EFFECTS->TriggerEffect(this, "bomb");
		if (!objBall)
			return;

		float sign = flip_x ? -1 : 1;
		float strength = 0.5;

		if (GetInput(PLAYERKEY_UP, controller_num))
			objBall->SetImpulse(0.0f, strength);

		else if (GetInput(PLAYERKEY_DOWN, controller_num))
			objBall->SetImpulse(0.0f, strength*0.1);

		else
			objBall->SetImpulse(sign * strength, strength / 3.0);
	}


	// ORIG PHYSICS TEST CODE:
	/*if (INPUT->KeyOnce(PLAYERKEY_ACTION1, controller_num) && 
		m_kPlayerState != WALKING_THRU_DOOR) 
	{	
		b2Body* pkBody = PHYSICS->CreateDynamicPhysicsBox(pos.x, pos.y, 15, 10);

		float sign = flip_x ? -1 : 1;
		float strength = 0.1;

		if (GetInput(PLAYERKEY_UP, controller_num))
			pkBody->ApplyImpulse(b2Vec2(0.0f, strength*1.7), pkBody->GetWorldCenter());

		else if (GetInput(PLAYERKEY_DOWN, controller_num))
			pkBody->ApplyImpulse(b2Vec2(0.0f, strength*1.7), pkBody->GetWorldCenter());

		else
			pkBody->ApplyImpulse(b2Vec2(sign * strength, strength / 3.0f), pkBody->GetWorldCenter());
	}

	return;
	*/

	// ORIGINAL CODE:

	/*
	if (INPUT->KeyOnce(PLAYERKEY_ACTION1, controller_num) && 
		m_kPlayerState != WALKING_THRU_DOOR) 
	{	
		int strength;
		if (!GLOBALS->Value("bomb_throw_strength", strength))
			return;

		Object* objBall = EFFECTS->TriggerEffect(this, "bomb");

		if (!objBall)
			return;

		float sign;
		if (flip_x)
			sign = -1;
		else
			sign = 1;

		if (GetInput(PLAYERKEY_UP, controller_num))
			objBall->SetVelXY(0.0f, vel.y + strength*1.7);

		else if (GetInput(PLAYERKEY_DOWN, controller_num))
			objBall->SetVelXY(0.0f, vel.y - strength);

		else
			objBall->SetVelXY(sign * strength + vel.x, vel.y + 6.0f);

		//objBall->SetVelXY(vel.x, 0.0f);
	}
	*/
}

void PlayerObject::LimitMaxHorizontalVelocityTo( float fMaxHorizontalVelocity )
{
	if (GetVelX() > fMaxHorizontalVelocity)
		SetVelX(fMaxHorizontalVelocity);

	if (GetVelX() < -fMaxHorizontalVelocity)
		SetVelX(-fMaxHorizontalVelocity);
}

void PlayerObject::LimitMaxVerticalVelocityTo( float fMaxVerticalVelocity )
{ 
	if (GetVelY() > fMaxVerticalVelocity)
		SetVelY(fMaxVerticalVelocity);

	if (GetVelY() < -fMaxVerticalVelocity)
		SetVelY(-fMaxVerticalVelocity);
}