#include "objectLayer.h"
#include "gameState.h"
#include "object.h"
#include "gameWorld.h"

void ObjectLayer::SetName(const char* _name) {
	SAFE_DELETE(name);
	name = new CString(_name);
}

void ObjectLayer::Draw() {
	ObjectListReverseIter rev_iter;
	Object* obj;

	// set scroll speed for this layer
	if (WORLD->UseScrollSpeed())
		WORLD->SetCameraScrollSpeed(scroll_speed);
	
	// ORDER IS IMPORTANT
	// we draw starting at the end, going to the beginning
	// things at the end were put there FIRST to be drawn FIRST.
	for (rev_iter = objects.rbegin(); rev_iter != objects.rend(); ++rev_iter) {
		obj = *rev_iter;

		assert(obj != NULL);

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
	assert(obj != NULL);
	ObjectListIter iter = find(objects.begin(), objects.end(), obj);

	if (iter != objects.end()) {
		*iter = NULL;
		objects.erase(iter);
	} else {
		fprintf(stderr, " WARN: ObjectLayer: asked to remove an"
										" object which isn't on this layer.\n");
	}
}

bool ObjectLayer::Init() {
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
	name = NULL;
	objects.clear();
}

ObjectLayer::~ObjectLayer() {
	Shutdown();
	SAFE_DELETE(name);
}

