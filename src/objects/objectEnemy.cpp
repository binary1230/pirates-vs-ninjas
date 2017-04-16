#include "stdafx.h"
#include "objectEnemy.h"

#include "globals.h"
#include "animation.h"
#include "gameState.h"
#include "gameSound.h"
#include "gameWorld.h"
#include "objectPlayer.h"

int ObjectEnemy::iSpawnedObjectCount = 0;

void ObjectEnemy::Shutdown() 
{
	BaseShutdown();
}

ObjectEnemy::ObjectEnemy() {}
ObjectEnemy::~ObjectEnemy() {}

bool ObjectEnemy::Init() 
{
	if (!BaseInit())
		return false;

	iTimeToWaitBeforeCollisionsAllowedAgain = 0;

	assert(WORLD->GetNumPlayers() > 0);
	m_pkTargetPlayer = WORLD->GetPlayer(0); 
	assert(m_pkTargetPlayer);

	properties.is_badguy = true;

	return true;
}

void ObjectEnemy::Update() 
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

void ObjectEnemy::OnCollide(Object* obj, const b2WorldManifold* pkbWorldManifold)
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

BOOST_CLASS_EXPORT_GUID(ObjectEnemy, "ObjectEnemy")