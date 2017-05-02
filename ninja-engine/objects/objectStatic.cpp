#include "stdafx.h"
#include "objectStatic.h"

#include "globals.h"
#include "animation.h"
#include "gameState.h"
#include "gameWorld.h"
#include "objectFactory.h"
#include "objectEnemy.h"


bool ObjectStatic::LoadObjectProperties(XMLNode &xDef) {
	if (!Object::LoadObjectProperties(xDef))
		return false;

	properties.is_static = 1;

	return true;
}

bool ObjectStatic::Init() {
	return BaseInit();
}

void ObjectStatic::Shutdown() {
	BaseShutdown();
}

void ObjectStatic::Update() {
	BaseUpdate();
	UpdateSimpleAnimations();

	UpdateSpawns();
}

void ObjectStatic::UpdateSpawns() 
{
	if (!properties.spawns_enemies)
		return;

#if BLOCKS_SPAWN_ENEMIES
	// experimental
	static int iSpawnWaitTime = 0;

	iSpawnWaitTime--;
	if (iSpawnWaitTime >= 0)
		return;

	iSpawnWaitTime = 60;

	if (ObjectEnemy::iSpawnedObjectCount > 100)
		return;

	ObjectEnemy::iSpawnedObjectCount++;

	Object* badyguy = OBJECT_FACTORY->CreateObject("enemy1");
	assert(badyguy);
	if (!badyguy)
		return;

	badyguy->SetLayer( GetLayer() );
	badyguy->SetXY(_Pos);
	badyguy->PlayAnimation(1);

	WORLD->AddObject(badyguy);
#endif BLOCKS_SPAWN_ENEMIES
}

ObjectStatic::ObjectStatic() {}
ObjectStatic::~ObjectStatic() {}

BOOST_CLASS_EXPORT_GUID(ObjectStatic, "ObjectStatic")