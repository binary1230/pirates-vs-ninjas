#include "stdafx.h"
#include "objectEnemy.h"

#include "globals.h"
#include "animation.h"
#include "game.h"
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
	if (ObjectPlayer* player = dynamic_cast<ObjectPlayer*>(obj))
	{
		SOUND->PlaySound("pop2");
		is_dead = true;
		return;
	}
}

BOOST_CLASS_EXPORT_GUID(ObjectEnemy, "ObjectEnemy")