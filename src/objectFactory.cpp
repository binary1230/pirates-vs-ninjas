#include "stdafx.h"
#include "objectFactory.h"

// TODO NOTES: ObjectFactory
// This class was recently refactored from gameWorld.h
// There are a LOT of things it does redundantly now that all
// the object creation info is stored here.  Fix them all.
//
// First:
// OBJECTID's are WRONG, they are more like OBJECT_TYPE_IDS
//
// OBJECTID's should not be types like "player" or "spring" or "static"
// but more like "sonic", "mario", "crazyspring43", "flower2"
//
// 2017 update: this class is about to get ripped to shreds.

#include "globals.h"
#include "gameState.h"
#include "sprite.h"
#include "object.h"
#include "objectIDs.h"

#ifdef USE_OLD_LOADING_SYSTEM
	#include "objectPlayer.h"
	#include "objectBounce.h"
	#include "objectBackground.h"
	#include "objectController.h"
	#include "objectStatic.h"
	#include "objectSpring.h"
	#include "objectCollectable.h"
	#include "objectFan.h"
	#include "objectDoor.h"
	#include "objectEnemy.h"
	#include "objectTxtOverlay.h"
	#include "objectCutBars.h"
#endif // USE_OLD_LOADING_SYSTEM

#include "assetManager.h"
#include "animations.h"
#include "gameSound.h"
#include "gameWorld.h"

DECLARE_SINGLETON(ObjectFactory)

bool ObjectFactory::AddObjectDefinition(const std::string &objDefName, 	
										const XMLNode &xObjectDef) {
	if (objDefName == "" || objDefName.length() < 1)
		return false;

	objectDefs[objDefName] = xObjectDef;

	return true;
}

XMLNode* ObjectFactory::FindObjectDefinition(const std::string &objDefName) {
	ObjectDefMappingIter iter = objectDefs.find(objDefName);

	if (iter == objectDefs.end())
		return NULL;
	
	return &(iter->second);
}

int ObjectFactory::GetObjectDefinitionCount() const {
	return objectDefs.size();
}

const std::string& ObjectFactory::GetObjectDefinition(int iIndex) const {
	assert(iIndex >= 0 && iIndex < (int)objectDefs.size());
	ObjectDefMappingConstIter iter = objectDefs.begin();
	
	// HACK: why?? this sucks. why does iter+= iIndex not work?
	for (int i = 0; i != iIndex; ++i)
		++iter;

	assert(iter != objectDefs.end());

	return iter->first;
}
	
//! Recursively loads Object Definitions from XML, 
//! puts them into an ObjectMapping
bool ObjectFactory::LoadObjectDefsFromXML(XMLNode &xObjDefs) {

	// Object definitions can take 2 forms in the XML file
	// 1) an <objectDef> tag
	// 2) an <include_xml_file> tag which we then open and get an <objectDef>
	
	int i, max, iterator;
	static const char* parent_include = "The Toplevel XML file";
	static int recurse_level = 0;

	XMLNode xObjectDef, xObjectDefFile;
	std::string objName, file;
	
	// 1) handle <objectDef> tags
	max = xObjDefs.nChildNode("objectDef");
	for (i = iterator = 0; i < max; i++) {
		xObjectDef = xObjDefs.getChildNode("objectDef", &iterator);
		objName = xObjectDef.getAttribute("name");
		
		if (!FindObjectDefinition(objName)) {
			AddObjectDefinition(objName, xObjectDef);
		} else {
			TRACE("ObjectFactory: WARNING: Duplicate object "
											"definitions found for object name: '%s', ignoring.\n",
											objName);
		}
	}

	// 2) handle <include_xml_file> tags 
	max = xObjDefs.nChildNode("include_xml_file");
	
	for (i = iterator = 0; i < max; i++) {
		file = xObjDefs.getChildNode("include_xml_file", &iterator).getText();
		if (!LoadObjectDefsFromIncludeXML(file)) {
			return false;
		}
	}

	return true;
}

bool ObjectFactory::LoadObjectDefsFromIncludeXML(std::string file) {
	std::string full_path = ASSETMANAGER->GetPathOf(file.c_str());

	if (!full_path.length()) {
		TRACE("ObjectFactory: ERROR: Can't open requested XML file for inclusion: '%s'\n", file);
		return false;
	}

	XMLNode xObjectDefFile = XMLNode::openFileHelper(full_path.c_str(), "objectDefinitions");

	return LoadObjectDefsFromXML(xObjectDefFile);
}

// XXX this shouldn't really be here...
void ObjectFactory::SetupTypes() {
	// maps strings of object types to numeric ID's.
	objectDefTypes["Bounce"] 			= OBJECT_ID_BOUNCE;
	objectDefTypes["Background"] 		= OBJECT_ID_BACKGROUND;
	objectDefTypes["Player"] 			= OBJECT_ID_PLAYER;
	objectDefTypes["ControllerDisplay"]	= OBJECT_ID_CONTROLLER;
	objectDefTypes["Static"] 			= OBJECT_ID_STATIC;
	objectDefTypes["Enemy"] 			= OBJECT_ID_ENEMY;
	objectDefTypes["Fan"]				= OBJECT_ID_FAN;
	objectDefTypes["Door"]				= OBJECT_ID_DOOR;
	objectDefTypes["Spring"]			= OBJECT_ID_SPRING;
	objectDefTypes["Collectable"]		= OBJECT_ID_COLLECTABLE;
	objectDefTypes["TextOverlay"]		= OBJECT_ID_TXTOVERLAY;
	objectDefTypes["CutBars"]			= OBJECT_ID_CUTBARS;
}

// Get the object ID from an XML object definition
ENGINE_OBJECTID ObjectFactory::GetObjectIDFromXML(XMLNode &xObjectDef) {
	return objectDefTypes[GetObjectTypeFromXML(xObjectDef)];
}

// Get the object ID from an XML object definition
std::string ObjectFactory::GetObjectTypeFromXML(XMLNode &xObjectDef) {
	return xObjectDef.getAttribute("type");
}

int ObjectFactory::Init() {
	objectDefs.clear();
	SetupTypes();
	return 0;
}

void ObjectFactory::Shutdown() {
	objectDefs.clear();
	objectDefTypes.clear();
}

ObjectFactory::ObjectFactory() {}
ObjectFactory::~ObjectFactory() {}

// everything below here is unused

#ifdef USE_OLD_LOADING_SYSTEM
Object* ObjectFactory::CreateObject(std::string objDefName) {
	XMLNode* xObjectDef = FindObjectDefinition(objDefName);

	if (!xObjectDef) {
		TRACE("ObjectFactory: Unable to instantiate "
			"object definition: '%s'\n", objDefName);
		return NULL;
	}

	ENGINE_OBJECTID id = GetObjectIDFromXML(*xObjectDef);

	Object* pkObject = CreateObject(id, *xObjectDef, NULL);
	if (!pkObject)
		return false;

	pkObject->SetObjectDefName(objDefName.c_str());
	return pkObject;
}

// Creates an object from an XML definition
// in: xObjectDef - XML representation of an object's definition
// in: xObject - XML representation of additional object paramaters
// returns: newly create Object*, or NULL if it failed
Object* ObjectFactory::CreateObjectFromXML(	
					XMLNode &xObjectDef, 
					XMLNode &xObject) 
{
	assert(WORLD != NULL);

	ENGINE_OBJECTID id = GetObjectIDFromXML(xObjectDef);

	if (id < 1) {
		TRACE("ObjectFactory: ERROR: Unable to find specified "
										"object type '%s'\n", 
										GetObjectTypeFromXML(xObjectDef) );
		return NULL;
	}
	
	Object* obj = CreateObject(id, xObjectDef, &xObject);

	if (!obj) {
		TRACE("ERROR: Unable to instantiate object of type: '%s'.\n",
										GetObjectTypeFromXML(xObjectDef) );
		return NULL;
	}
	
	return obj;
}

Object* ObjectFactory::CreateObject(ENGINE_OBJECTID id, 
									XMLNode &xObjectDef,
									XMLNode *xObject) {
	Object* obj = NULL;

	switch(id) {
			
		// not working yet.		
		//case OBJECT_ID_3D:
		//	obj = New3dObject(xObjectDef, xObject);
		//	break;

		case OBJECT_ID_BACKGROUND:
			obj = NewBackgroundObject(xObjectDef, xObject);
			break;
			
		case OBJECT_ID_PLAYER:
			obj = NewPlayerObject(xObjectDef, xObject);
			break;
			
		case OBJECT_ID_BOUNCE:
			obj = NewBounceObject(xObjectDef, xObject);
			break;
			
		case OBJECT_ID_CONTROLLER:
			obj = NewControllerObject(xObjectDef, xObject);
			break;

		case OBJECT_ID_STATIC:
			obj = NewStaticObject(xObjectDef, xObject);
			break;

		case OBJECT_ID_SPRING:
			obj = NewSpringObject(xObjectDef, xObject);
			break;

		case OBJECT_ID_ENEMY:
			obj = NewEnemyObject(xObjectDef, xObject);
			break;

		case OBJECT_ID_COLLECTABLE:
			obj = NewCollectableObject(xObjectDef, xObject);
			break;

		case OBJECT_ID_DOOR:
			obj = NewDoorObject(xObjectDef, xObject);
			break;

		case OBJECT_ID_FAN:
			obj = NewFanObject(xObjectDef, xObject);
			break;
		
		case OBJECT_ID_TXTOVERLAY:
			obj = NewTxtOverlayObject(xObjectDef, xObject);
			break;

		case OBJECT_ID_CUTBARS:
			obj = NewCutBarObject(xObjectDef, xObject);
			break;

		case 0:
			obj = NULL;
			break;
			
		default:
			TRACE("ERROR: Unknown Object ID passed?? [%i]\n", id);
			obj = NULL;
			break;
	}

	if (obj && xObject) 
		obj->SetObjectDefName(xObject->getAttribute("objectDef"));

	return obj;
}


//! Factory method, creates new PlayerObjects from XML files
//
//! NOTE: this only takes an ObjectDefinition XML fragment,
// memory leaks on failures here.. CLEAN IT.
Object* ObjectFactory::NewPlayerObject(XMLNode &xDef, XMLNode *xObj) {
	
	PlayerObject* obj = new PlayerObject();
	if (!obj->LoadFromObjectDef(xDef))
		return NULL;
	
	return obj;
}

Object* ObjectFactory::NewCutBarObject(XMLNode &xDef, XMLNode *xObj) {
	
	CutBarObject* obj = new CutBarObject();
	if (!obj->LoadFromObjectDef(xDef))
		return NULL;

	if (xObj && xObj->nChildNode("text"))
		obj->SetText(xObj->getChildNode("text").getText());
	
	obj->Start();
	
	return obj;
}

Object* ObjectFactory::NewBounceObject(XMLNode &xDef, XMLNode *xObj) {
	
	ObjectBounce* obj = new ObjectBounce();
	if (!obj->LoadFromObjectDef(xDef))
		return NULL;

	return obj;
}

Object* ObjectFactory::NewCollectableObject(XMLNode &xDef, XMLNode *xObj) {
	
	CollectableObject* obj = new CollectableObject();
	if (!obj->LoadFromObjectDef(xDef))
		return NULL;
	
	return obj;
}

Object* ObjectFactory::NewTxtOverlayObject(XMLNode &xDef, XMLNode *xObj) {

	ObjectText* obj = new ObjectText();	
	if (!obj->LoadFromObjectDef(xDef))
		return NULL;

	if (!xObj) 
		return obj;

	std::string txt, avatar;

	if (xObj->nChildNode("text"))
		txt = xObj->getChildNode("text").getText();

	if (xObj->nChildNode("avatar"))
		avatar = xObj->getChildNode("avatar").getText();
	
	obj->SetText(txt);
	obj->SetAvatarFilename(avatar);

	return obj;
}

Object* ObjectFactory::NewControllerObject(XMLNode &xDef, XMLNode *xObj) {
 
 	ObjectController* obj = new ObjectController();
	if (!obj->LoadFromObjectDef(xDef))
		return NULL;

	// XXX READ which controller we monitor from XML file
	// but not in this method

	int i, iterator, max;
	std::string filename;
	XMLNode xImages, xBtn;
	  
	xImages = xDef.getChildNode("images");
	max = xImages.nChildNode("btn");

	obj->buttons.clear();
	obj->buttons.resize(max);

	filename = xImages.getChildNode("base").getText();
	  
	obj->controller_sprite = ASSETMANAGER->LoadSprite(filename.c_str());
	    
	if (!obj->controller_sprite) {
		TRACE("-- ERROR: Can't load file '%s'\n", filename );
		delete obj;
		return NULL;
	}
	  
	int x1,y1;
	if (!xImages.getChildNode("base").getChildNode("x").getInt(x1)) {
		TRACE("Invalid controller base X!\n");
		return NULL;
	}
	if (!xImages.getChildNode("base").getChildNode("x").getInt(y1)) {
		TRACE("Invalid controller base Y!\n");
		return NULL;
	}
	obj->pos.x = x1;
	obj->pos.y = y1;

	Button* b;
  
	// load each button
	max = obj->buttons.size();
	for (i=iterator=0; i < max; i++) {
		xBtn = xImages.getChildNode("btn", &iterator);
		b = &obj->buttons[i];

		filename = xBtn.getText();
		b->active = 0;

		b->sprite = ASSETMANAGER->LoadSprite(filename.c_str());

		if (!b->sprite) {
			TRACE("-- ERROR: Can't load file '%s'\n",filename);
			return NULL;
		}

		int x2,y2;  
		if (!xBtn.getChildNode("x").getInt(x2)) {
			TRACE("Invalid controller button X!\n");
			return NULL;
		}
		if (!xBtn.getChildNode("y").getInt(y2)) {
			TRACE("Invalid controller button Y!\n");
			return NULL;
		}
		b->sprite->x_offset = x2;
		b->sprite->y_offset = y2;
	}

	if (xDef.nChildNode("showDuringDemoOnly") > 0)
		obj->only_show_during_demo = true;
	
	return obj;
}

Object* ObjectFactory::NewBackgroundObject(XMLNode &xDef, XMLNode *xObj) {
 
	BackgroundObject* obj = new BackgroundObject();	
	if (!obj->LoadFromObjectDef(xDef))
		return NULL;

	return obj;
}

Object* ObjectFactory::NewStaticObject(XMLNode &xDef, XMLNode *xObj) {
	
	StaticObject* obj = new StaticObject();
	if (!obj->LoadFromObjectDef(xDef))
		return NULL;

	return obj;
}

Object* ObjectFactory::NewEnemyObject(XMLNode &xDef, XMLNode *xObj) 
{
	EnemyObject* obj = new EnemyObject();
	if (!obj->LoadFromObjectDef(xDef))
		return NULL;

	return obj;
}

Object* ObjectFactory::NewSpringObject(XMLNode &xDef, XMLNode *xObj) 
{ 
	SpringObject* obj = new SpringObject();
	if (!obj->LoadFromObjectDef(xDef))
		return NULL;

	XMLNode xSpringDirection;
	bool using_default = true;

	// order we search for the spring strength:
	// 1) Object Instance
	// 2) Object definition
	// 3) default value of (DEFAULT_SPRING_STRENGTH)
	if (xObj && xObj->nChildNode("springDirection") == 1) {
		xSpringDirection = xObj->getChildNode("springDirection");
		using_default = false;
	} else if (xDef.nChildNode("springDirection") == 1) {
		xSpringDirection = xDef.getChildNode("springDirection");
		using_default = false;
	}

	obj->spring_vector.x = DEFAULT_SPRING_STRENGTH_X;
	obj->spring_vector.y = DEFAULT_SPRING_STRENGTH_Y;

	if (!using_default) {
		if ( xSpringDirection.nChildNode("x") != 1 ||  
				!xSpringDirection.getChildNode("x").getFloat(obj->spring_vector.x)) {
			TRACE(" -- invalid spring strength (x)!\n");
			return false;
		}
	
		if ( xSpringDirection.nChildNode("y") != 1 ||  
				!xSpringDirection.getChildNode("y").getFloat(obj->spring_vector.y)) {
			TRACE(" -- invalid spring strength (y)!\n");
			return NULL;
		}
	}

	return obj;
}

#define DEFAULT_DOOR_TYPE "exit"

Object* ObjectFactory::NewDoorObject(XMLNode &xDef, XMLNode *xObj) 
{	
	ObjectDoor* obj = new ObjectDoor();
	if (!obj->LoadFromObjectDef(xDef))
		return NULL;

	// doors have 3 attributes they can use:
	//
	// type - the type of this door (level exit, warp, return to last mode, etc)
	// 
	// the following are used for the appropriate types:
	//
	// name - name of this door, used when jumping back to it from another mode 
	//        (like jumping back to a door outside after exiting a house)
	//
	// modeToTrigger - the name of the mode to trigger when this door activates
	
	if (!xObj)
		return obj;

	std::string door_type = "";
	if (xObj->getAttribute("type"))
		door_type = xObj->getAttribute("type");

	if (door_type.length() == 0)
		door_type = DEFAULT_DOOR_TYPE;

	if (door_type == "exit")
		obj->door_type = LEVEL_EXIT;
	else if (door_type == "warp")
		obj->door_type = WARP_TO_ANOTHER_PORTAL;
	else if (door_type == "switchToNewMode")
		obj->door_type = SWITCH_TO_ANOTHER_MODE;
	else if (door_type == "return")
		obj->door_type = RETURN_TO_LAST_MODE;
	else
		obj->door_type = INVALID_TYPE;

	if (xObj->getAttribute("name"))
		obj->door_name = xObj->getAttribute("name");

	if (xObj->getAttribute("modeToTrigger"))
		obj->mode_to_jump_to_on_activate = xObj->getAttribute("modeToTrigger");
	
	return obj;
}

Object* ObjectFactory::NewFanObject(XMLNode &xDef, XMLNode *xObj) 
{	
	ObjectFan* obj = new ObjectFan();
	if (!obj->LoadFromObjectDef(xDef))
		return NULL;

	return obj;
}
#endif // USE_OLD_LOADING_SYSTEM