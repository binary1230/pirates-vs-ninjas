#include "objectLayer.h"
#include "gameState.h"
#include "xmlParser.h"
#include "StdString.h"
#include "object.h"
#include "physSimulation.h"

void ObjectLayer::Draw() {
	int i, max = objects.size();

	// set scroll speed for this layer
	GetGameState()->GetPhysSimulation()->SetCameraScrollSpeed(scroll_speed);
	
	for (i=0; i<max; i++) {
		objects[i]->Draw();
	}
}

void ObjectLayer::PushObject(Object* obj) {
	objects.push_back(obj);
}

bool ObjectLayer::Init(GameState* gs) {
	scroll_speed = 1;
	visible = true;
	SetGameState(gs);
	objects.clear();
	return true;
}

void ObjectLayer::Shutdown() {
	// we do NOT delete these Object's, as they are held somewhere else
	objects.clear();	
	SetGameState(NULL);
}

ObjectLayer::ObjectLayer() {
	scroll_speed = 1.0f;
	visible = true;	
	objects.clear();
}

ObjectLayer::~ObjectLayer() {}

