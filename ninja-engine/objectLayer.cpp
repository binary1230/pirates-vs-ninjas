#include "stdafx.h"
#include "objectLayer.h"
#include "game.h"
#include "object.h"
#include "gameWorld.h"

/*
void ObjectLayer::Draw() {
	if (!IsVisible())
		return;

	Object* obj;

	// ORDER IS IMPORTANT
	// we draw starting at the end, going to the beginning
	// things at the end were put there FIRST to be drawn FIRST.
	for (auto rev_iter = _objects.rbegin(); rev_iter != _objects.rend(); ++rev_iter) {
		obj = *rev_iter;

		assert(obj != NULL);

		if (!obj->IsDead())
			obj->Draw();
	}
}

// Change the order of the object in the list of objects
// Objects that are at the front of the list will be drawn over items on the back of the list
void ObjectLayer::ReorderObject(Object* obj, bool move_backwards) {
	for (auto it = _objects.begin(); it != _objects.end();) {
		if (*it != obj) {
			++it;
			continue;
		}

		it = _objects.erase(it);

		if (move_backwards) {
			if (it != _objects.begin()))
				it--;
			if (it != _objects.begin())
				it--;
		} else {
			if (it != _objects.end())
				it++;
		}

		_objects.insert(it, obj);
		return;
	}
}

void ObjectLayer::AddObject(Object* obj) {
	_objects.push_front(obj);
}

// Does not free any memory, just removes from our list
// This is a bit silly... layers need rethinking.
void ObjectLayer::RemoveObject(Object* obj) {
	assert(obj != NULL);
	auto iter = find(_objects.begin(), _objects.end(), obj);

	if (iter != _objects.end()) {
		*iter = NULL;
		_objects.erase(iter);
	} else {
		TRACE(" WARN: ObjectLayer: asked to remove an object which isn't on this layer.\n");
	}
}*/

bool ObjectLayer::Init() {
	scroll_speed = 1;
	visible = true;
	return true;
}

void ObjectLayer::Shutdown() {
	// we do NOT delete these Object's, as they are held somewhere else
}

ObjectLayer::ObjectLayer() {
	scroll_speed = 1.0f;
	visible = true;
}

ObjectLayer::~ObjectLayer() {
	Shutdown();
}

BOOST_CLASS_EXPORT_GUID(ObjectLayer, "ObjectLayer")