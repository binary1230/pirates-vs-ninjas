#include "objectFactory.h"

// TODO NOTES: ObjectFactory
// This class was recently refactored from physSimulation.h
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
#include "objectPlayer.h"
#include "objectRadiusBlock.h"
#include "objectBackground.h"
#include "objectController.h"
#include "objectStatic.h"
#include "objectSpring.h"
#include "objectCollectable.h"
#include "objectFan.h"
#include "objectDoor.h"
#include "assetManager.h"
#include "animations.h"
#include "gameSound.h"

DECLARE_SINGLETON(ObjectFactory)
		
Object* ObjectFactory::CreateObject(CString objDefName) {
	XMLNode* xObjectDef = FindObjectDefinition(objDefName);

	if (!xObjectDef) {
		fprintf(stderr, "ObjectFactory: Unable to instantiate "
										"object definition: '%s'\n", objDefName.c_str());
		return NULL;
	}
	
	OBJECTID id = GetObjectIDFromXML(*xObjectDef);
	
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

	
//! Loads Object Definitions from XML, puts them in an ObjectMapping
int ObjectFactory::LoadObjectDefsFromXML(XMLNode &xObjDefs) {

	// Object definitions can take 2 forms in the XML file
	// 1) an <objectDef> tag
	// 2) an <include_xml_file> tag which we then open and get an <objectDef>
	
	int i, max, iterator;
	XMLNode xObjectDef;
	CString objName, file;
	
	// 1) handle <objectDef> tags
	max = xObjDefs.nChildNode("objectDef");
	iterator = 0;
	for (i = iterator = 0; i < max; i++) {
		xObjectDef = xObjDefs.getChildNode("objectDef", &iterator);
		objName = xObjectDef.getAttribute("name");
		
		if (!FindObjectDefinition(objName)) {
			AddObjectDefinition(objName, xObjectDef);
		} else {
			fprintf(stderr, "ObjectFactory: ERROR: Duplicate object "
											"definitions found for object name: '%s'\n", 
											objName.c_str());
			return 0;
		}
	}

	// 2) handle <include_xml_file> tags (more common)
	max = xObjDefs.nChildNode("include_xml_file");
	
	for (i = iterator = 0; i < max; i++) {
					
		// get the filename
		file = xObjDefs.getChildNode("include_xml_file", &iterator).getText();
		
		// open that file, get the objectDef
		file = ASSETMANAGER->GetPathOf(file);
		xObjectDef = XMLNode::openFileHelper(file.c_str(), "objectDef");

		// save it
		objName = xObjectDef.getAttribute("name");

		if (!FindObjectDefinition(objName)) {
			AddObjectDefinition(objName, xObjectDef);
		} else {
			fprintf(stderr, "ObjectFactory: ERROR: Duplicate object "
											"definitions found for object name: '%s'\n", 
											objName.c_str());
			return 0;
		}
	}

	return 1;
}

// XXX this shouldn't really be here...
void ObjectFactory::SetupTypes() {
	// maps strings of object types to numeric ID's.
	objectDefTypes["RadiusBlock"] 			= OBJECT_ID_RADIUS_BLOCK;
	objectDefTypes["Background"] 				= OBJECT_ID_BACKGROUND;
	objectDefTypes["Player"] 						= OBJECT_ID_PLAYER;
	objectDefTypes["ControllerDisplay"]	= OBJECT_ID_CONTROLLER;
	objectDefTypes["Static"] 						= OBJECT_ID_STATIC;
	objectDefTypes["Fan"]								= OBJECT_ID_FAN;
	objectDefTypes["Door"]							= OBJECT_ID_DOOR;
	objectDefTypes["Spring"]						= OBJECT_ID_SPRING;
	objectDefTypes["Collectable"]				= OBJECT_ID_COLLECTABLE;
}

// Get the object ID from an XML object definition
OBJECTID ObjectFactory::GetObjectIDFromXML(XMLNode &xObjectDef) {
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
	assert(physSimulation);

	OBJECTID id = GetObjectIDFromXML(xObjectDef);

	if (id < 1) {
		fprintf(stderr, "ObjectFactory: ERROR: Unable to find specified "
										"object type '%s'\n", 
										GetObjectTypeFromXML(xObjectDef).c_str() );
		return NULL;
	}
	
	Object* obj = CreateObject(id, xObjectDef, &xObject);

	if (!obj) {
		fprintf(stderr, "ERROR: Unable to instantiate object of type: '%s'.\n",
										GetObjectTypeFromXML(xObjectDef).c_str() );
		return NULL;
	}
	
	return obj;
}

Object* ObjectFactory::CreateObject(	OBJECTID id, 
																			XMLNode &xObjectDef,
																			XMLNode *xObject) {
	Object* obj = NULL;

	switch(id) {
					
		case OBJECT_ID_BACKGROUND:
			obj = NewBackgroundObject(xObjectDef, xObject);
			break;
			
		case OBJECT_ID_PLAYER:
			obj = NewPlayerObject(xObjectDef, xObject);
			break;
			
		case OBJECT_ID_RADIUS_BLOCK:
			obj = NewRadiusBlockObject(xObjectDef, xObject);
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

		case 0:
			obj = NULL;
			break;
			
		default:
			fprintf(stderr, "ERROR: Unknown Object ID passed?? [%i]\n", id);
			obj = NULL;
			break;
	}

	return obj;
}

int ObjectFactory::Init() {
	physSimulation = NULL;
	objectDefs.clear();
	SetupTypes();
	return 0;
}

void ObjectFactory::Shutdown() {
	physSimulation = NULL;
	objectDefs.clear();
	objectDefTypes.clear();
}

//! Factory method, creates new PlayerObjects from XML files
//
//! NOTE: this only takes an ObjectDefinition XML fragment,
// memory leaks on failures here.. CLEAN IT.
Object* ObjectFactory::NewPlayerObject(XMLNode &xDef, XMLNode *xObj) {
	
	// ObjectProperties props;
	PlayerObject* obj = new PlayerObject();

	// init the object
	if (!obj || !obj->Init(physSimulation) )
		return NULL;

	// load the animations
	AnimationMapping animation_map = GetPlayerAnimationMappings();
	if (!LoadObjectAnimations(obj, xDef, &animation_map))
		return NULL;

	// load the sounds
	if (!LoadObjectSounds(obj,xDef))
		return NULL;
	
	// get the object properties
	if (!LoadObjectProperties(obj, xDef))
		return NULL;

	if (!obj->LoadPlayerProperties(xDef))
		return NULL;

	obj->SetupCachedVariables();

	return obj;
}

Object* ObjectFactory::NewRadiusBlockObject(XMLNode &xDef, XMLNode *xObj) {
	RadiusBlockObject* obj = new RadiusBlockObject();

  if (!obj || !obj->Init(physSimulation) )
    return NULL;

  obj->SetTheta(Rand(0,360));
  obj->SetRadius(Rand(20,300));

  if (!LoadObjectAnimations(obj, xDef))
    return NULL;

  if (!LoadObjectProperties(obj, xDef))
    return NULL;
	
	obj->SetupCachedVariables();

  return obj;
}

Object* ObjectFactory::NewCollectableObject(XMLNode &xDef, XMLNode *xObj) {
  CollectableObject* obj = new CollectableObject();

  if (!obj || !obj->Init(physSimulation) )
    return NULL;

  if (!LoadObjectSounds(obj,xDef))
    return NULL;
  
  if (!LoadObjectAnimations(obj,xDef))
    return NULL;
  
  if (!LoadObjectProperties(obj, xDef))
    return NULL;

  obj->properties.is_collectable = 1;
  obj->properties.is_ring = 1;

	obj->SetupCachedVariables();

  return obj;
}

Object* ObjectFactory::NewControllerObject(XMLNode &xDef, XMLNode *xObj) {
  ControllerObject* obj = new ControllerObject();

  if (!obj || !obj->Init(physSimulation) )
    return NULL;

  ObjectProperties props;
  props.is_overlay = 1;
  obj->SetProperties(props);

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
  obj->controller_sprite = new Sprite();
  
  obj->controller_sprite->bmp = 
  ASSETMANAGER->LoadBitmap(filename.c_str());
    
  if (!obj->controller_sprite->bmp) {
    fprintf(stderr, "-- ERROR: Can't load file '%s'\n", filename.c_str() );
    delete obj->controller_sprite;
    delete obj;
    return NULL;
  }
  
  obj->controller_sprite->x_offset = 0;
  obj->controller_sprite->y_offset = 0;

  int x1,y1;
  if (!xImages.getChildNode("base").getChildNode("x").getInt(x1)) {
    fprintf(stderr, "Invalid controller base X!\n");
    return NULL;
  }
  if (!xImages.getChildNode("base").getChildNode("x").getInt(y1)) {
    fprintf(stderr, "Invalid controller base Y!\n");
    return NULL;
  }
  obj->pos.SetX(x1);
  obj->pos.SetY(y1);

  Button* b;
  
  // load each button
  max = obj->buttons.size();
  for (i=iterator=0; i < max; i++) {
      xBtn = xImages.getChildNode("btn", &iterator);
      b = &obj->buttons[i];
      
      filename = xBtn.getText();
      b->sprite = new Sprite();
      b->active = 0;
    
      b->sprite->bmp = ASSETMANAGER->LoadBitmap(filename.c_str());
      
      if (!b->sprite->bmp) {
        fprintf(stderr,"-- ERROR: Can't load file '%s'\n",filename.c_str());
        return NULL;
      }

      int x2,y2;  
      if (!xBtn.getChildNode("x").getInt(x2)) {
        fprintf(stderr, "Invalid controller button X!\n");
        return NULL;
      }
      if (!xBtn.getChildNode("y").getInt(y2)) {
        fprintf(stderr, "Invalid controller button Y!\n");
        return NULL;
      }
      b->sprite->x_offset = x2;
      b->sprite->y_offset = y2;
  }

  if (xDef.nChildNode("showDuringDemoOnly") > 0)
    obj->only_show_during_demo = true;
  
  if (!LoadObjectProperties(obj, xDef))
    return NULL;
	
	obj->SetupCachedVariables();
  
  return obj;
}

Object* ObjectFactory::NewBackgroundObject(XMLNode &xDef, XMLNode *xObj) {
  BackgroundObject* obj = new BackgroundObject();
  
  if (!obj || !obj->Init(physSimulation) )
    return NULL;

  obj->SetXY(0,0);

  if (!LoadObjectAnimations(obj,xDef) )
    return NULL;
  
  if (!LoadObjectProperties(obj, xDef))
    return NULL;
  
	obj->SetupCachedVariables();

  return obj;
}

Object* ObjectFactory::NewStaticObject(XMLNode &xDef, XMLNode *xObj) {
	StaticObject* obj = new StaticObject();

  if (!obj || !obj->Init(physSimulation) )
    return NULL;

  if (!LoadObjectAnimations(obj,xDef))
    return NULL;

  if (!LoadObjectProperties(obj, xDef))
    return NULL;
	
	obj->SetupCachedVariables();

	return obj;
}

Object* ObjectFactory::NewSpringObject(XMLNode &xDef, XMLNode *xObj) { 
  SpringObject* obj = new SpringObject();
  obj->properties.spring_strength = 20; // default

  if (!obj || !obj->Init(physSimulation) )
    return NULL;

  if (!LoadObjectSounds(obj,xDef))
    return NULL;
  
  if (!LoadObjectAnimations(obj,xDef))
    return NULL;
  
  if (!LoadObjectProperties(obj, xDef))
    return NULL;

  obj->properties.is_spring = 1;
	
	obj->SetupCachedVariables();

  return obj;
}

Object* ObjectFactory::NewDoorObject(XMLNode &xDef, XMLNode *xObj) {
	DoorObject* obj = new DoorObject();

  if (!obj || !obj->Init(physSimulation) )
    return NULL;

  if (!LoadObjectAnimations(obj,xDef))
    return NULL;

  if (!LoadObjectProperties(obj, xDef))
    return NULL;

	obj->properties.is_door = 1;
	obj->properties.is_solid = 1;
	
	obj->SetupCachedVariables();

	return obj;
}

Object* ObjectFactory::NewFanObject(XMLNode &xDef, XMLNode *xObj) {
	FanObject* obj = new FanObject();

  if (!obj || !obj->Init(physSimulation) )
    return NULL;

  if (!LoadObjectAnimations(obj,xDef))
    return NULL;

  if (!LoadObjectProperties(obj, xDef))
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
			fprintf(stderr, "-- Invalid MASS.\n");
			return false;
		}
	}
	
	ClearProperties(obj->properties);
	obj->properties.feels_gravity = xProps.nChildNode("affectedByGravity"); 
	obj->properties.feels_user_input = xProps.nChildNode("affectedByInput1"); 
	obj->properties.feels_friction = xProps.nChildNode("affectedByFriction"); 

	obj->properties.is_solid = xProps.nChildNode("solidObject");
	
	if (xProps.nChildNode("isOverlay")) {
		obj->properties.is_overlay = 1;
	}

	if ( xProps.nChildNode("springStrength") && 
			!xProps.getChildNode("springStrength").getInt(
			obj->properties.spring_strength)		) {

		fprintf(stderr, " -- invalid spring strength!\n");
		return false;
	}

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
		
		anim = Animation::New(xAnim);

		if (!anim) {
			return false;
		} else {
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
	}

	// set the default animation XXX error check?
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


ObjectFactory::ObjectFactory() {}
ObjectFactory::~ObjectFactory() {} 
