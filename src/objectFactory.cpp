#include "objectFactory.h"

#include "globals.h"
#include "object.h"
#include "objectIDs.h"
#include "objectPlayer.h"
#include "objectRadiusBlock.h"
#include "objectBackground.h"
#include "objectController.h"
#include "objectStatic.h"
#include "StdString.h"

#include <map>

using namespace std;

// Creates an object from an XML definition
// in: xObjectDef - XML representation of an object's definition
// in: xObject - XML representation of additional object paramaters
// returns: newly create Object*, or NULL if it failed
Object* ObjectFactory::CreateObject(XMLNode &xObjectDef, XMLNode &xObject) {

	Object* obj = NULL;

	// XXX this shouldn't really be here...
	// maps strings of object types to numeric ID's.
	map<const CString, uint> types;
	types["RadiusBlock"] 				= OBJECT_ID_RADIUS_BLOCK;
	types["Background"] 				= OBJECT_ID_BACKGROUND;
	types["Player"] 						= OBJECT_ID_PLAYER;
	types["ControllerDisplay"] 	= OBJECT_ID_CONTROLLER;
	types["Static"] 						= OBJECT_ID_STATIC;
	
	CString objType = xObjectDef.getAttribute("type");
	
	uint id = types[objType];

	switch(id) {
					
		case OBJECT_ID_BACKGROUND:
			obj = BackgroundObject::New(GetGameState(), xObjectDef, xObject);
			break;
			
		case OBJECT_ID_PLAYER:
			obj = PlayerObject::New(GetGameState(), xObjectDef, xObject);
			break;
			
		case OBJECT_ID_RADIUS_BLOCK:
			obj = RadiusBlockObject::New(GetGameState(), xObjectDef, xObject);
			break;
			
		case OBJECT_ID_CONTROLLER:
			obj = ControllerObject::New(GetGameState(), xObjectDef, xObject);
			break;

		case OBJECT_ID_STATIC:
			obj = StaticObject::New(GetGameState(), xObjectDef, xObject);
			break;

		case 0:
			fprintf(stderr, "ERROR: Specified object doesn't exist - '%s'.\n",
											objType.c_str());
			break;
			
		default:
			fprintf(stderr, "ERROR: Unknown Object ID passed?? [%i]\n", id);
	}

	return obj;
}

int ObjectFactory::Init(GameState* _game_state) {
	SetGameState(_game_state);
	return 0;
}

void ObjectFactory::Shutdown() {
	SetGameState(NULL);
}

ObjectFactory::ObjectFactory() {}
ObjectFactory::~ObjectFactory() {} 
