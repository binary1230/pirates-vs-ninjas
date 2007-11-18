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

#include "globals.h"
#include "gameState.h"
#include "sprite.h"
#include "object.h"
#include "objectIDs.h"
#include "objectPlayerHuman.h"
#include "objectBounce.h"
#include "objectBackground.h"
#include "objectController.h"
#include "objectStatic.h"
#include "objectSpring.h"
#include "objectCollectable.h"
#include "objectFan.h"
#include "objectDoor.h"
#include "objectTxtOverlay.h"
// #include "object3d.h" // not yet.
#include "objectCutBars.h"
#include "assetManager.h"
#include "animations.h"
#include "gameSound.h"
#include "gameWorld.h"

DECLARE_SINGLETON(ObjectFactory)
		
Object* ObjectFactory::CreateObject(CString objDefName) {
	XMLNode* xObjectDef = FindObjectDefinition(objDefName);

	if (!xObjectDef) {
		TRACE("ObjectFactory: Unable to instantiate "
										"object definition: '%s'\n", objDefName.c_str());
		return NULL;
	}
	
	ENGINE_OBJECTID id = GetObjectIDFromXML(*xObjectDef);
	
	return CreateObject(id, *xObjectDef, NULL);
}

bool ObjectFactory::AddObjectDefinition(const CString &objDefName, 	
																				const XMLNode &xObjectDef) {
	if (objDefName == "" || objDefName.length() < 1)
		return false;

	objectDefs[objDefName] = xObjectDef;

	return true;
}

XMLNode* ObjectFactory::FindObjectDefinition(const CString &objDefName) {
	ObjectDefMappingIter iter = objectDefs.find(objDefName);

	if (iter == objectDefs.end())
		return NULL;
	
	return &(iter->second);
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
	CString objName, file;
	
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
											objName.c_str());
		}
	}

	// 2) handle <include_xml_file> tags 
	max = xObjDefs.nChildNode("include_xml_file");
	
	for (i = iterator = 0; i < max; i++) {
					
		// get the filename
		file = xObjDefs.getChildNode("include_xml_file", &iterator).getText();
		
		// open that file, get the objectDef
		CString fileNew = ASSETMANAGER->GetPathOf(file);
	
		if (!fileNew.size()) {
			TRACE("ObjectFactory: ERROR: Can't open "
											"requested XML file for inclusion: '%s'\n", 
											file.c_str() );
			return false;
		}	

		// this method is recursive, let's make sure
		// we don't fall into any infinite loops.
		if (++recurse_level > 99) {
			TRACE(	"ERROR: Infinite loop while reading object\n"
												"       definitions!!  Make sure that that\n"
												"       '%s' does not include itself!\n", 
												parent_include);

			return false;
		}
		
		parent_include = fileNew.c_str();

		xObjectDefFile = XMLNode::openFileHelper(	fileNew.c_str(), 
																							"objectDefinitions");

		// recursively call ourself to handle this
		if (!LoadObjectDefsFromXML(xObjectDefFile))
			return false;

		--recurse_level;
	}

	return true;
}

// XXX this shouldn't really be here...
void ObjectFactory::SetupTypes() {
	// maps strings of object types to numeric ID's.
	objectDefTypes["Bounce"] 						= OBJECT_ID_BOUNCE;
	objectDefTypes["Background"] 				= OBJECT_ID_BACKGROUND;
	objectDefTypes["Player"] 						= OBJECT_ID_PLAYER;
	objectDefTypes["ControllerDisplay"]	= OBJECT_ID_CONTROLLER;
	objectDefTypes["Static"] 						= OBJECT_ID_STATIC;
	objectDefTypes["Fan"]								= OBJECT_ID_FAN;
	objectDefTypes["Door"]							= OBJECT_ID_DOOR;
	objectDefTypes["Spring"]						= OBJECT_ID_SPRING;
	objectDefTypes["Collectable"]				= OBJECT_ID_COLLECTABLE;
	objectDefTypes["TextOverlay"]				= OBJECT_ID_TXTOVERLAY;
	objectDefTypes["CutBars"]						= OBJECT_ID_CUTBARS;
}

// Get the object ID from an XML object definition
ENGINE_OBJECTID ObjectFactory::GetObjectIDFromXML(XMLNode &xObjectDef) {
	return objectDefTypes[GetObjectTypeFromXML(xObjectDef)];
}

// Get the object ID from an XML object definition
CString ObjectFactory::GetObjectTypeFromXML(XMLNode &xObjectDef) {
	return xObjectDef.getAttribute("type");
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
										GetObjectTypeFromXML(xObjectDef).c_str() );
		return NULL;
	}
	
	Object* obj = CreateObject(id, xObjectDef, &xObject);

	if (!obj) {
		TRACE("ERROR: Unable to instantiate object of type: '%s'.\n",
										GetObjectTypeFromXML(xObjectDef).c_str() );
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
			obj = NewHumanPlayerObject(xObjectDef, xObject);
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

int ObjectFactory::Init() {
	objectDefs.clear();
	SetupTypes();
	return 0;
}

void ObjectFactory::Shutdown() {
	objectDefs.clear();
	objectDefTypes.clear();
}

//! Factory method, creates new PlayerObjects from XML files
//
//! NOTE: this only takes an ObjectDefinition XML fragment,
// memory leaks on failures here.. CLEAN IT.
Object* ObjectFactory::NewHumanPlayerObject(XMLNode &xDef, XMLNode *xObj) {
	
	HumanPlayerObject* obj = new HumanPlayerObject();
	
	if (!LoadCommonObjectStuff(obj, xDef, xObj, false))
		return NULL;
	
	AnimationMapping animation_map = GetPlayerAnimationMappings();
	if (!LoadObjectAnimations(obj, xDef, &animation_map))
		return NULL;

	if (!obj->LoadPlayerProperties(xDef))
		return NULL;

	// need to do it again to catch the new animations
	obj->SetupCachedVariables();

	return obj;
}

Object* ObjectFactory::NewCutBarObject(XMLNode &xDef, XMLNode *xObj) {
	CutBarObject* obj = new CutBarObject();

	if (!LoadCommonObjectStuff(obj, xDef, xObj, false))
		return NULL;

	if (xObj && xObj->nChildNode("text"))
		obj->SetText(xObj->getChildNode("text").getText());
	
	obj->properties.is_overlay = true;
	obj->Start();

	obj->SetupCachedVariables();
	
	return obj;
}

Object* ObjectFactory::NewBounceObject(XMLNode &xDef, XMLNode *xObj) {
	
	ObjectBounce* obj = new ObjectBounce();
	if (!LoadCommonObjectStuff(obj, xDef, xObj))
		return NULL;

	obj->properties.is_ball = 1;
	obj->properties.is_solid = 1;

	obj->SetupCachedVariables();
	return obj;
}

Object* ObjectFactory::NewCollectableObject(XMLNode &xDef, XMLNode *xObj) {
	
	CollectableObject* obj = new CollectableObject();
	if (!LoadCommonObjectStuff(obj, xDef, xObj))
		return NULL;
	
	obj->properties.is_collectable = 1;
	obj->properties.is_ring = 1;

	obj->SetupCachedVariables();
	
	return obj;
}

Object* ObjectFactory::NewTxtOverlayObject(XMLNode &xDef, XMLNode *xObj) {

	CString txt, avatar;
	ObjectText* obj = new ObjectText();	
	if (!LoadCommonObjectStuff(obj, xDef, xObj, false))
		return NULL;

	if (!xObj) 
		return obj;

	if (xObj->nChildNode("text"))
		txt = xObj->getChildNode("text").getText();

	if (xObj->nChildNode("avatar"))
		avatar = xObj->getChildNode("avatar").getText();
	
	obj->SetText(txt);
	obj->SetAvatarFilename(avatar);

	obj->SetupCachedVariables();

	return obj;
}

Object* ObjectFactory::NewControllerObject(XMLNode &xDef, XMLNode *xObj) {
 
 	ObjectController* obj = new ObjectController();
	if (!LoadCommonObjectStuff(obj, xDef, xObj, false))
		return NULL;

	obj->properties.is_overlay = 1;

	// XXX READ which controller we monitor from XML file
	// but not in this method

	int i, iterator, max;
	CString filename;
	XMLNode xImages, xBtn;
	  
	xImages = xDef.getChildNode("images");
	max = xImages.nChildNode("btn");

	obj->buttons.clear();
	obj->buttons.resize(max);

	filename = xImages.getChildNode("base").getText();
	  
	obj->controller_sprite = ASSETMANAGER->LoadSprite(filename.c_str());
	    
	if (!obj->controller_sprite) {
		TRACE("-- ERROR: Can't load file '%s'\n", filename.c_str() );
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
			TRACE("-- ERROR: Can't load file '%s'\n",filename.c_str());
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
	
	obj->SetupCachedVariables();
	return obj;
}

Object* ObjectFactory::NewBackgroundObject(XMLNode &xDef, XMLNode *xObj) {
 
	BackgroundObject* obj = new BackgroundObject();	
	if (!LoadCommonObjectStuff(obj, xDef, xObj))
		return NULL;
	
	obj->SetXY(0,0);
	obj->SetupCachedVariables();

	obj->m_bCanCollide = false;

	return obj;
}

Object* ObjectFactory::NewStaticObject(XMLNode &xDef, XMLNode *xObj) {
	
	StaticObject* obj = new StaticObject();
	if (!LoadCommonObjectStuff(obj, xDef, xObj))
		return NULL;

	obj->SetupCachedVariables();
	return obj;
}

//Object* ObjectFactory::New3dObject(XMLNode &xDef, XMLNode *xObj) {
//
//	ModelObject* obj = new ModelObject();
//
//	if (!LoadCommonObjectStuff(obj, xDef, xObj))
//		return NULL;
//	
//	obj->SetupCachedVariables();
//	return obj;
//}

Object* ObjectFactory::NewSpringObject(XMLNode &xDef, XMLNode *xObj) { 
  
	XMLNode xSpringDirection;
	bool using_default = true;

	SpringObject* obj = new SpringObject();
	if (!LoadCommonObjectStuff(obj, xDef, xObj))
		return NULL;
  
	obj->properties.is_spring = 1;

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

	float x_strength = DEFAULT_SPRING_STRENGTH_X;
	float y_strength = DEFAULT_SPRING_STRENGTH_Y;

	if (!using_default) {
		if ( xSpringDirection.nChildNode("x") != 1 ||  
				!xSpringDirection.getChildNode("x").getFloat(x_strength)) {
			TRACE(" -- invalid spring strength (x)!\n");
			return false;
		}
	
		if ( xSpringDirection.nChildNode("y") != 1 ||  
				!xSpringDirection.getChildNode("y").getFloat(y_strength)) {
			TRACE(" -- invalid spring strength (y)!\n");
			return NULL;
		}
	}

	obj->spring_vector.x = x_strength;
	obj->spring_vector.y = y_strength;

	obj->SetupCachedVariables();

	return obj;
}

#define DEFAULT_DOOR_TYPE "exit"

Object* ObjectFactory::NewDoorObject(XMLNode &xDef, XMLNode *xObj) {
	
	DoorObject* obj = new DoorObject();
	
	if (!LoadCommonObjectStuff(obj, xDef, xObj, false))
		return NULL;	
	
	AnimationMapping animation_map = GetDoorAnimationMappings();
	if (!LoadObjectAnimations(obj, xDef, &animation_map))
		return NULL;

	obj->properties.is_door = 1;
	obj->properties.is_solid = 1;
	
	obj->SetupCachedVariables();

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

	CString door_type = xObj->getAttribute("type");

	if (door_type.size() == 0)
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

	obj->door_name = xObj->getAttribute("name");
	obj->mode_to_jump_to_on_activate = xObj->getAttribute("modeToTrigger");
	
	return obj;
}

Object* ObjectFactory::NewFanObject(XMLNode &xDef, XMLNode *xObj) {
	
	FanObject* obj = new FanObject();
	
	if (!LoadCommonObjectStuff(obj, xDef, xObj))
		return NULL;

	obj->properties.is_fan = 1;
	obj->properties.is_solid = 1;

	obj->SetupCachedVariables();
	return obj;
}

//! Load some common object properties from XML
bool ObjectFactory::LoadObjectProperties(Object* obj, XMLNode &xDef) {
	XMLNode xProps = xDef.getChildNode("properties");

	if (xProps.nChildNode("mass")) {
		if (!xProps.getChildNode("mass").getFloat(obj->mass)) {
			TRACE("-- Invalid MASS.\n");
			return false;
		}
	}
	
	ClearProperties(obj->properties);
	obj->properties.feels_gravity = xProps.nChildNode("affectedByGravity") != 0; 
	obj->properties.feels_user_input = xProps.nChildNode("affectedByInput1") != 0; 
	obj->properties.feels_friction = xProps.nChildNode("affectedByFriction") != 0; 

	obj->properties.is_solid = xProps.nChildNode("solidObject") != 0;
	
	if (xProps.nChildNode("isOverlay")) {
		obj->properties.is_overlay = 1;
	}

	/*if ( xProps.nChildNode("springStrength") && 
			!xProps.getChildNode("springStrength").getInt(
			obj->properties.spring_strength)		) {

		TRACE(" -- invalid spring strength!\n");
		return false;
	}*/

	return true;
}

// A helper function to load animations
bool ObjectFactory::LoadObjectAnimations(
			Object* obj, XMLNode &xDef, 
			AnimationMapping *animation_lookup) {

	int i, iterator, max;

	Animation* anim = NULL;
	CString anim_name;
	XMLNode xAnim, xAnims;
	
	xAnims = xDef.getChildNode("animations");
	max = xAnims.nChildNode("animation");
	
	obj->animations.resize(max);

	for (i=iterator=0; i<max; i++) {
		xAnim = xAnims.getChildNode("animation", &iterator);
		anim_name = xAnim.getAttribute("name");
	
		// Load the animation	
		anim = Animation::Load(xAnim, obj);

		if (!anim)
			return false;

		// if we have animation names (e.g. "walking") then use them to figure
		// out which index we store this animation at
		// if not, just put it in the next available index
		int index;
			
		if (animation_lookup)
			index = (*animation_lookup)[anim_name];
		else 
			index = i;
			
		obj->animations[index] = anim;
	}

	// set the default animation 
	// XXX error check this!
	
	CString default_name;
	int default_index; 

	if (!animation_lookup) {
		obj->currentAnimation = obj->animations[0];
	} else { 
		default_name = xAnims.getAttribute("default");
		default_index = (*animation_lookup)[default_name];
		obj->currentAnimation = obj->animations[default_index];
	}

	// set the current sprite to the first frame of the animation
	obj->currentSprite = obj->currentAnimation->GetCurrentSprite();

	return true;
}

//! Load any sounds specified in the XML
// (obj not used)
bool ObjectFactory::LoadObjectSounds(Object* obj, XMLNode &xDef) {
				
	if (xDef.nChildNode("sounds")) {
		XMLNode xSounds = xDef.getChildNode("sounds");
		if (!SOUND->LoadSounds(xSounds))
			return false;
	}

	return true;
}

bool ObjectFactory::LoadCommonObjectStuff(Object* obj,
										  XMLNode &xDef, 
										  XMLNode *xObj,
										  bool loadAnimations) {

	if (!obj || !obj->Init() )
		return false;
		
	if (!LoadObjectProperties(obj, xDef))
		return false;	

	if (!LoadObjectSounds(obj,xDef))
		return false;

	if (loadAnimations && !LoadObjectAnimations(obj,xDef))
		return false;

	obj->SetupCachedVariables();

	return true;
}	

ObjectFactory::ObjectFactory() {}
ObjectFactory::~ObjectFactory() {} 
