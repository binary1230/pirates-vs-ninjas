#include "objectLayer.h"
#include "gameState.h"
#include "object.h"
#include "physSimulation.h"

void ObjectLayer::Draw() {
	ObjectListReverseIter rev_iter;
	Object* obj;

	// set scroll speed for this layer
	simulation->SetCameraScrollSpeed(scroll_speed);
	
	// ORDER IS IMPORTANT
	// we draw starting at the end, going to the beginning
	// things at the end were put there FIRST to be drawn FIRST.
	for (rev_iter = objects.rbegin(); rev_iter != objects.rend(); rev_iter++) {
		obj = *rev_iter;

		if (!obj->IsDead())
			obj->Draw();
	}
}

void ObjectLayer::AddObject(Object* obj) {
	objects.push_front(obj);
}

// Does not free any memory, just removes from our list
// This is a bit silly... layers need rethinking.
void ObjectLayer::RemoveObject(Object* obj) {
	ObjectListIter iter = find(objects.begin(), objects.end(), obj);

	if (iter != objects.end())
		objects.erase(iter);
	else
		fprintf(stderr, " WARN: memmgmt: asked to remove an object that's not here.\n");
}

bool ObjectLayer::Init(PhysSimulation* p) {
	simulation = p;
	scroll_speed = 1;
	visible = true;
	objects.clear();
	return true;
}

void ObjectLayer::Shutdown() {
	// we do NOT delete these Object's, as they are held somewhere else
	objects.clear();	
}

ObjectLayer::ObjectLayer() {
	scroll_speed = 1.0f;
	visible = true;	
	simulation = NULL;
	objects.clear();
}

ObjectLayer::~ObjectLayer() {
	Shutdown();
	simulation = NULL;
}

