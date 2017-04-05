#include "stdafx.h"
#include "mapSaver.h"
#include "gameWorld.h"
#include "object.h"
#include "window.h"
#include "gameSound.h"
#include "objectLayer.h"
#include "objectSpring.h"
#include "objectDoor.h"

MapSaver::MapSaver() {
	simulation = NULL;
}

MapSaver::~MapSaver() {}

bool MapSaver::SaveEverything(	const GameWorld* p, 
								const CString file, 
								XMLNode &xObjDefs) {

	assert(p != NULL && file.GetLength() > 0);

	simulation = p;

	TRACE("MAPEDITOR: Starting to save to '%s'...\n", file);

	XMLNode xMode = XMLNode::createXMLTopNode("gameMode");
	xMode.addAttribute("type", "simulation");

	if (simulation->m_szMusicFile.GetLength())
		xMode.addChild("music").addText(simulation->m_szMusicFile);

	if (simulation->m_szLuaScript.GetLength())
		xMode.addChild("luaScript").addText(simulation->m_szLuaScript);

	XMLNode xInfo = xMode.addChild("info");
	//xInfo.addChild("map_version").addText(simulation->map
	//xInfo.addChild("author").addText(
	//xInfo.addChild("description").addText(

	XMLNode xProps = xMode.addChild("properties");
	xProps.addChild("width").addText(simulation->m_iLevelWidth);
	xProps.addChild("height").addText(simulation->m_iLevelHeight);
	
	XMLNode xBgColor = xProps.addChild("bgcolor");
	xBgColor.addChild("r").addText(getr(simulation->m_bgColor));
	xBgColor.addChild("g").addText(getg(simulation->m_bgColor));
	xBgColor.addChild("b").addText(getb(simulation->m_bgColor));

	if (simulation->m_bgColorTop != -1) {
		XMLNode xBgColor2 = xProps.addChild("bgcolor_top");
		xBgColor2.addChild("r").addText(getr(simulation->m_bgColorTop));
		xBgColor2.addChild("g").addText(getg(simulation->m_bgColorTop));
		xBgColor2.addChild("b").addText(getb(simulation->m_bgColorTop));
	}

	xMode.addChild(xObjDefs);	// "objectDefinitions" node
	xMode.addChild(simulation->m_xEffects);

	XMLNode xMap = xMode.addChild("map");

	OutputLayers(xMap);

	xMode.writeToFile(file.c_str());
	
	TRACE("MAPEDITOR: Finished save!\n");

	simulation = NULL;

	return true;
}

void MapSaver::OutputObject(const Object* obj, XMLNode &xObj) {
	assert(obj);

	if (obj->objectDefName)
		xObj.addAttribute("objectDef", obj->objectDefName->c_str());
				
	XMLNode xPos = xObj.addChild("position");
	xPos.addAttribute("type", "fixed");		// all position types are FIXED for XML writes.

	// compensate for scroll speed now.
	int x = int( float(obj->pos.x) * obj->GetLayer()->GetScrollSpeed() );
	int y = int( float(obj->pos.y) * obj->GetLayer()->GetScrollSpeed() );

	xPos.addChild("x").addText(x);
	xPos.addChild("y").addText(y);

	if (obj->flip_y)
		xPos.addChild("flipy");

	if (obj->flip_x)
		xPos.addChild("flipx");

	if (obj->GetDebugFlag())
		xPos.addChild("debug");

	/*
	if (obj->vel.x != 0.0f)
		xPos.addChild("velx").addText(obj->vel.x);

	if (obj->vel.y != 0.0f)
		xPos.addChild("velx").addText(obj->vel.y);
	*/

	if (obj->alpha != 255)
		xPos.addChild("alpha").addText(obj->alpha);

	if (obj->fade_out_time_total > 0)
		xPos.addChild("fadeout").addText(obj->fade_out_time_total);

	if (obj->rotate_velocity != 0)
		xPos.addChild("vel_rotate").addText(obj->rotate_velocity);

	if (obj == simulation->m_pkCameraLookatTarget)
		xObj.addChild("cameraFollow");

	if (obj->controller_num != 0)
		xObj.addChild("inputController").addText(obj->controller_num);

	// handle springs
	if (obj->properties.is_spring) {
		SpringObject* objSpring = (SpringObject*)obj;		// pray
		
		XMLNode xSpring = xObj.addChild("springDirection");
		xSpring.addChild("x").addText(objSpring->spring_vector.x);
		xSpring.addChild("y").addText(objSpring->spring_vector.y);
	}

	// handle doors
	if (obj->properties.is_door) {
		DoorObject* objDoor = (DoorObject*)obj;	// pray

		assert(objDoor->door_type != INVALID_TYPE);

		char* door_type = NULL;

		if (objDoor->door_type == LEVEL_EXIT)
			door_type = "exit";
		else if (objDoor->door_type == WARP_TO_ANOTHER_PORTAL)
			door_type = "warp";
		else if (objDoor->door_type == SWITCH_TO_ANOTHER_MODE)
 			door_type = "switchToNewMode";
		else if (objDoor->door_type == RETURN_TO_LAST_MODE)
 			door_type = "return";

		assert(door_type && "ERROR: Unknown door type specified.");

		xObj.addAttribute("type", door_type);
		
		if (objDoor->door_name.size() > 0)
			xObj.addAttribute(	"name", 
													objDoor->door_name.c_str());

		if (objDoor->mode_to_jump_to_on_activate.size() > 0)
			xObj.addAttribute(	"modeToTrigger", 
													objDoor->mode_to_jump_to_on_activate.c_str() );
	}
}

void MapSaver::OutputLayer(const ObjectLayer* layer, XMLNode &xLayer) {
	assert(layer);

	xLayer.addAttribute("scroll_speed", layer->scroll_speed);
	if (layer->name)
		xLayer.addAttribute("name", layer->name.c_str());

	ObjectListConstReverseIter iter;
	XMLNode xObject;
	for (iter = layer->objects.rbegin(); iter != layer->objects.rend(); ++iter) {
		xObject = xLayer.addChild("object");
		OutputObject(*iter, xObject);
	}
}

void MapSaver::OutputLayers(XMLNode &xMap) {
	uint i, max = simulation->m_kLayers.size();
	XMLNode xLayer;
	for (i = 0; i < max; ++i) {
		xLayer = xMap.addChild("layer");
		OutputLayer(simulation->m_kLayers[i], xLayer);
	}
}
