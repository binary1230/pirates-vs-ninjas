#include "stdafx.h"
#include "objectEnemy.h"

#include "globals.h"
#include "animation.h"
#include "gameState.h"
#include "gameSound.h"
#include "gameWorld.h"
#include "objectPlayer.h"

int EnemyObject::iSpawnedObjectCount = 0;

void EnemyObject::Shutdown() 
{
	BaseShutdown();
}

EnemyObject::EnemyObject() {}
EnemyObject::~EnemyObject() {}

bool EnemyObject::Init() 
{
	if (!BaseInit())
		return false;

	m_bCanCollide = true;
	iTimeToWaitBeforeCollisionsAllowedAgain = 0;

	assert(WORLD->GetNumPlayers() > 0);
	m_pkTargetPlayer = WORLD->GetPlayer(0); 
	assert(m_pkTargetPlayer);

	properties.is_badguy = true;

	return true;
}

void EnemyObject::Update() 
{
	BaseUpdate();
	UpdateSimpleAnimations();

	if (bCollidedLastFrame)
	{
		//SetVelXY(m_kCollisionDirection);
	}

	if (iTimeToWaitBeforeCollisionsAllowedAgain > 0)
		iTimeToWaitBeforeCollisionsAllowedAgain--;

	bCollidedLastFrame = false;

	flip_x = GetVelX() > 0;
}

void EnemyObject::OnCollide(Object* obj, const b2ContactPoint* pkContactPoint) 
{
	if (obj->GetProperties().is_player) 
	{
		SOUND->PlaySound("pop2");
		is_dead = true;
		return;
	}

	/*if (obj->GetProperties().is_badguy)
	{
		if (bCollidedLastFrame == false && iTimeToWaitBeforeCollisionsAllowedAgain == 0)
		{
			//SOUND->PlaySound("ring");
			m_kCollisionDirection = obj->GetVelXY();
			iTimeToWaitBeforeCollisionsAllowedAgain = 60;
		}

		bCollidedLastFrame = true;
	}*/
}

void EnemyObject::ApplyForces()
{
	MoveTowardsPlayer();
}

void EnemyObject::MoveTowardsPlayer()
{
	const float fMoveSpeed = 0.05f;
	const float fMinThresholdDistance = 100.0f;
	const float fMaxThresholdDistance = 800.0f;

	const float fMinThresholdDistanceSquared = fMinThresholdDistance * fMinThresholdDistance;
	const float fMaxThresholdDistanceSquared = fMaxThresholdDistance * fMaxThresholdDistance;
	Vector2D kDirection = m_pkTargetPlayer->GetXY() - pos;

	float fSign = 1.0f;

	if (kDirection.LengthSquared() < fMinThresholdDistanceSquared)
		fSign *= -6.0f;
	else
		fSign = 1.0f;

	/*if (kDirection.LengthSquared() > fMaxThresholdDistanceSquared)
	{
		vel *= 0.8f;
	}*/

/*	if (kDirection.x > 0)
		accel.x = fSign * fMoveSpeed;
	else
		accel.x = -fSign * fMoveSpeed;

	if (kDirection.y > 0)
		accel.y = fSign * fMoveSpeed;
	else
		accel.y = -fSign * fMoveSpeed;*/
}