#include "mapSaver.h"
#include "physSimulation.h"
#include "object.h"
#include "window.h"
#include "gameSound.h"
#include "objectLayer.h"

MapSaver::MapSaver() {}
MapSaver::~MapSaver() {}

bool MapSaver::SaveEverything(const PhysSimulation* p, const CString file) {
	assert(p != NULL && file.size() > 0);

	fprintf(stderr, "MAPEDITOR: Starting to save to '%s'...\n", file.c_str());

	XMLNode xMode = XMLNode::createXMLTopNode("gameMode");
	xMode.addAttribute("type", "simulation");

	XMLNode xProps = xMode.addChild("properties");
	xProps.addChild("width").addText(p->width);
	xProps.addChild("height").addText(p->height);
	xProps.addChild("camera_x").addText(p->camera_x);
	xProps.addChild("camera_y").addText(p->camera_y);

	XMLNode xObjDefs = xMode.addChild("objectDefinitions");

	XMLNode xMap = xMode.addChild("map");

	OutputLayers(p, xMap);

	xMode.writeToFile(file.c_str());
	
	fprintf(stderr, "MAPEDITOR: Finished save!");

	return true;
}

void MapSaver::OutputObject(const Object* obj, XMLNode &xObj) {
	if (obj->objectDefName)
		xObj.addAttribute("objectDef", obj->objectDefName->c_str());
				
	XMLNode xPos = xObj.addChild("position");

	xPos.addAttribute("type", "fixed");		// all position types are FIXED
																				// for XML writes.
	
	xPos.addChild("x").addText(obj->pos.GetX());
	xPos.addChild("y").addText(obj->pos.GetY());

	// TODO: velocity (MARK)
	// rotational
	// camera follow
	// inputController
	// doors
	// CRAPLOADS OF SHIT
}

void MapSaver::OutputLayer(const ObjectLayer* layer, XMLNode &xLayer) {
	xLayer.addAttribute("scroll_speed", layer->scroll_speed);
	if (layer->name)
		xLayer.addAttribute("name", layer->name->c_str());

	ObjectConstListIter iter;
	XMLNode xObject;
	for (iter = layer->objects.begin(); iter != layer->objects.end(); ++iter) {
		xObject = xLayer.addChild("object");
		OutputObject(*iter, xObject);
	}
}

void MapSaver::OutputLayers(const PhysSimulation* p, XMLNode &xMap) {
	uint i, max = p->layers.size();
	XMLNode xLayer;
	for (i = 0; i < max; ++i) {
		xLayer = xMap.addChild("layer");
		OutputLayer(p->layers[i], xLayer);
	}
}
