#include "objectFactory.h"

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
#include "assetManager.h"
#include "StdString.h"
#include "animations.h"
#include "gameSound.h"

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
	types["Spring"]							= OBJECT_ID_SPRING;
	types["Collectable"]				= OBJECT_ID_COLLECTABLE;
	
	CString objType = xObjectDef.getAttribute("type");
	
	uint id = types[objType];

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

//! Factory method, creates new PlayerObjects from XML files
//
//! NOTE: this only takes an ObjectDefinition XML fragment,
// memory leaks on failures here.. CLEAN IT.
Object* ObjectFactory::NewPlayerObject(XMLNode &xDef, XMLNode &xObj) {
	
	// ObjectProperties props;
	PlayerObject* obj = new PlayerObject();

	// init the object
	if (!obj || !obj->Init(GetGameState()) )
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

	return obj;
}

Object* ObjectFactory::NewRadiusBlockObject(XMLNode &xDef, XMLNode &xObj) {
	RadiusBlockObject* obj = new RadiusBlockObject();

  if (!obj || !obj->Init(GetGameState()) )
    return NULL;

  obj->SetTheta(Rand(0,360));
  obj->SetRadius(Rand(20,300));

  if (!LoadObjectAnimations(obj, xDef))
    return NULL;

  if (!LoadObjectProperties(obj, xDef))
    return NULL;

  return obj;
}

Object* ObjectFactory::NewCollectableObject(XMLNode &xDef, XMLNode &xObj) {
  CollectableObject* obj = new CollectableObject();

  if (!obj || !obj->Init(GetGameState()) )
    return NULL;

  if (!LoadObjectSounds(obj,xDef))
    return NULL;
  
  if (!LoadObjectAnimations(obj,xDef))
    return NULL;
  
  if (!LoadObjectProperties(obj, xDef))
    return NULL;

  obj->properties.is_collectable = 1;

  return obj;
}

Object* ObjectFactory::NewControllerObject(XMLNode &xDef, XMLNode &xObj) {
  ControllerObject* obj = new ControllerObject();

  if (!obj || !obj->Init(GetGameState()) )
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
  GetGameState()->GetAssetManager()->LoadBitmap(filename.c_str());
    
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
    
      b->sprite->bmp = 
      GetGameState()->GetAssetManager()->LoadBitmap(filename.c_str());
      
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
  
  return obj;
}

Object* ObjectFactory::NewBackgroundObject(XMLNode &xDef, XMLNode &xObj) {
  BackgroundObject* obj = new BackgroundObject();
  
  if (!obj || !obj->Init(GetGameState()) )
    return NULL;

  obj->SetXY(0,0);

  if (!LoadObjectAnimations(obj,xDef) )
    return NULL;
  
  if (!LoadObjectProperties(obj, xDef))
    return NULL;
  
  return obj;
}

Object* ObjectFactory::NewStaticObject(XMLNode &xDef, XMLNode &xObj) {
	StaticObject* obj = new StaticObject();

  if (!obj || !obj->Init(GetGameState()) )
    return NULL;

  if (!LoadObjectAnimations(obj,xDef))
    return NULL;

  if (!LoadObjectProperties(obj, xDef))
    return NULL;

	return obj;
}

Object* ObjectFactory::NewSpringObject(XMLNode &xDef, XMLNode &xObj) { 
  SpringObject* obj = new SpringObject();
  obj->properties.spring_strength = 20; // default

  if (!obj || !obj->Init(GetGameState()) )
    return NULL;

  if (!LoadObjectSounds(obj,xDef))
    return NULL;
  
  if (!LoadObjectAnimations(obj,xDef))
    return NULL;
  
  if (!LoadObjectProperties(obj, xDef))
    return NULL;

  obj->properties.is_spring = 1;

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
		
		anim = Animation::New(GetGameState(), xAnim);

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

	// set the default animation XXX error check? how?
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
				
	GameSound* sound = GetGameState()->GetSound();

	if (xDef.nChildNode("sounds")) {
		XMLNode xSounds = xDef.getChildNode("sounds");
		if (!sound->LoadSounds(xSounds))
			return false;
	}

	return true;
}


ObjectFactory::ObjectFactory() {}
ObjectFactory::~ObjectFactory() {} 
