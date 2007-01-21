#include "mapSaver.h"
#include "physSimulation.h"
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

	assert(p != NULL && file.size() > 0);

	simulation = p;

	fprintf(stderr, "MAPEDITOR: Starting to save to '%s'...\n", file.c_str());

	XMLNode xMode = XMLNode::createXMLTopNode("gameMode");
	xMode.addAttribute("type", "simulation");

	if (simulation->music_file)
	xMode.addChild("music").addText(simulation->music_file);

	XMLNode xInfo = xMode.addChild("info");
	//xInfo.addChild("map_version").addText(
	//xInfo.addChild("author").addText(
	//xInfo.addChild("description").addText(

	XMLNode xProps = xMode.addChild("properties");
	xProps.addChild("width").addText(simulation->width);
	xProps.addChild("height").addText(simulation->height);
	
	XMLNode xBgColor = xProps.addChild("bgcolor");
	xBgColor.addChild("r").addText(getr(simulation->bg_color));
	xBgColor.addChild("g").addText(getg(simulation->bg_color));
	xBgColor.addChild("b").addText(getb(simulation->bg_color));

	xMode.addChild(xObjDefs);	// "objectDefinitions" node

	XMLNode xMap = xMode.addChild("map");

	OutputLayers(xMap);

	xMode.writeToFile(file.c_str());
	
	fprintf(stderr, "MAPEDITOR: Finished save!\n");

	simulation = NULL;

	return true;
}

void MapSaver::OutputObject(const Object* obj, XMLNode &xObj) {
	assert(obj);

	if (obj->objectDefName)
		xObj.addAttribute("objectDef", obj->objectDefName->c_str());
				
	XMLNode xPos = xObj.addChild("position");

	xPos.addAttribute("type", "fixed");		// all position types are FIXED
																				// for XML writes.
	
	xPos.addChild("x").addText(obj->pos.GetX());
	xPos.addChild("y").addText(obj->pos.GetY());

	if (obj->flip_y)
		xPos.addChild("flipy");

	if (obj->flip_x)
		xPos.addChild("flipx");

	if (obj == simulation->camera_follow)
		xObj.addChild("cameraFollow");

	if (obj->controller_num != 0)
		xObj.addChild("inputController").addText(obj->controller_num);

	// handle springs
	if (obj->properties.is_spring) {
		SpringObject* objSpring = (SpringObject*)obj;		// pray
		
		XMLNode xSpring = xObj.addChild("springDirection");
		xSpring.addChild("x").addText(objSpring->spring_vector.GetX());
		xSpring.addChild("y").addText(objSpring->spring_vector.GetY());
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

		assert(door_type && "ERROR: Unkown door type specified.");

		xObj.addAttribute("type", door_type);
		
		if (objDoor->door_name.size() > 0)
			xObj.addAttribute(	"name", 
													objDoor->door_name.c_str());

		if (objDoor->mode_to_jump_to_on_activate.size() > 0)
			xObj.addAttribute(	"modeToTrigger", 
													objDoor->mode_to_jump_to_on_activate.c_str() );
	}

	// TODO: 
	// velocity 
	// rotational velocity
	// inputController
	// CRAPLOADS OF OTHER STUFF
}

void MapSaver::OutputLayer(const ObjectLayer* layer, XMLNode &xLayer) {
	assert(layer);

	xLayer.addAttribute("scroll_speed", layer->scroll_speed);
	if (layer->name)
		xLayer.addAttribute("name", layer->name->c_str());

	ObjectListConstReverseIter iter;
	XMLNode xObject;
	for (iter = layer->objects.rbegin(); iter != layer->objects.rend(); ++iter) {
		xObject = xLayer.addChild("object");
		OutputObject(*iter, xObject);
	}
}

void MapSaver::OutputLayers(XMLNode &xMap) {
	uint i, max = simulation->layers.size();
	XMLNode xLayer;
	for (i = 0; i < max; ++i) {
		xLayer = xMap.addChild("layer");
		OutputLayer(simulation->layers[i], xLayer);
	}
}
