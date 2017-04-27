#include "stdafx.h"
#include "objectSpring.h"

#include "globals.h"
#include "animation.h"
#include "gameState.h"
#include "gameSound.h"
#include "physics.h"
#include "objectPlayer.h"

#define DEFAULT_SPRING_RESET_TIME 6

void ObjectSpring::Shutdown() {
	BaseShutdown();
}

void ObjectSpring::Update() {
	BaseUpdate();
	UpdateSimpleAnimations();

	if (spring_reset_time == DEFAULT_SPRING_RESET_TIME)
		spring_is_active = false;

	if (spring_reset_time > 0) {
		spring_reset_time--;
		return;
	}

	if (spring_reset_time == 0) {
		spring_is_active = true;
	}

	/*else
		spring_is_active = true;

	// We were just sprung last Collide()
	if (!spring_is_active && spring_reset_time == 0)
		spring_reset_time = DEFAULT_SPRING_RESET_TIME;*/
}

bool ObjectSpring::LoadSpringVectorFromXML(XMLNode &xSpringDirection)
{
	if (xSpringDirection.nChildNode("x") != 1 || !xSpringDirection.getChildNode("x").getFloat(spring_vector.x)) {
		TRACE(" -- invalid spring strength (x)!\n");
		return false;
	}

	if (xSpringDirection.nChildNode("y") != 1 || !xSpringDirection.getChildNode("y").getFloat(spring_vector.y)) {
		TRACE(" -- invalid spring strength (y)!\n");
		return false;
	}

	return true;
}

bool ObjectSpring::LoadXMLInstanceProperties(XMLNode & xObj)
{
	if (xObj.nChildNode("springDirection") == 1) {
		if (!LoadSpringVectorFromXML(xObj.getChildNode("springDirection"))) {
			return false;
		}
	}

	return true;
}

bool ObjectSpring::LoadObjectProperties(XMLNode &xDef) {
	if (!Object::LoadObjectProperties(xDef))
		return false;

	properties.uses_physics_engine = 1;
	properties.is_static = 1;
	properties.is_sensor = 1;

	/*#if USE_OLD_LOADING_SYSTEM
	// we should be able to re-enable this later, there's some bugginess with load ordering and defaults when using
	// the serialized data.
	if (xDef.nChildNode("springDirection") == 1) {
		if (!LoadSpringVectorFromXML(xDef.getChildNode("springDirection"))) {
			return false;
		}
	}
	#endif*/

	return true;
}

bool ObjectSpring::Init() {
	spring_reset_time = 0;
	spring_is_active = true;

	return BaseInit();
}

void ObjectSpring::Clear() {
	spring_vector.x = DEFAULT_SPRING_STRENGTH_X;
	spring_vector.y = DEFAULT_SPRING_STRENGTH_Y;
}

ObjectSpring::ObjectSpring() { Clear(); }
ObjectSpring::~ObjectSpring() {}

void ObjectSpring::OnCollide(Object* obj, const b2WorldManifold* pkbWorldManifold) {
	if (ObjectPlayer* player = dynamic_cast<ObjectPlayer*>(obj)) {
		if (spring_reset_time == 0) {
			currentAnimation->Unfreeze();
			spring_reset_time = DEFAULT_SPRING_RESET_TIME;
		}
	}
}

BOOST_CLASS_EXPORT_GUID(ObjectSpring, "ObjectSpring")