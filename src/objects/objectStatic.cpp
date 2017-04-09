#include "stdafx.h"
#include "objectStatic.h"

#include "globals.h"
#include "animation.h"
#include "gameState.h"
#include "gameWorld.h"
#include "objectFactory.h"
#include "objectEnemy.h"

bool StaticObject::Init() {
	return BaseInit();
}

void StaticObject::Shutdown() {
	BaseShutdown();
}

void StaticObject::Update() {
	BaseUpdate();
	UpdateSimpleAnimations();

	UpdateSpawns(); // HACK, stupid.
}

// TOTAL HACK DONT CHECK IN ENEMY TESTING ONLY
void StaticObject::UpdateSpawns() 
{
	if (!properties.spawns_enemies)
		return;

	// HACK:
	return;

	static int iSpawnWaitTime = 0;

	iSpawnWaitTime--;
	if (iSpawnWaitTime >= 0)
		return;

	iSpawnWaitTime = 60;

	if (EnemyObject::iSpawnedObjectCount > 100)
		return;

	EnemyObject::iSpawnedObjectCount++;

	Object* badyguy = OBJECT_FACTORY->CreateObject("enemy1");
	assert(badyguy);
	if (!badyguy)
		return;

	badyguy->SetLayer( GetLayer() );
	badyguy->SetXY(pos);
	badyguy->PlayAnimation(1);

	WORLD->AddObject(badyguy);
}

StaticObject::StaticObject() {
	junk_test = 42;
}
StaticObject::~StaticObject() {}

BOOST_CLASS_EXPORT_GUID(StaticObject, "StaticObject")