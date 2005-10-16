#include "object.h"

void Object::Draw() {
}

void Object::Update() {
}

void Object::SetDestinationBitmap(BITMAP* _dst_bitmap) {
				dst_bitmap = _dst_bitmap;
}

void Object::SetBitmap(BITMAP* _bitmap) {
				bitmap = _bitmap;
}
				
//Object::Object(BITMAP* _sprite, BITMAP* _destination) : x(0),y(0),sprite(_sprite),destination(_destination) {}
// Object::Object(BITMAP* _sprite) : x(0),y(0),sprite(_sprite) {}

Object::Object() : x(0), y(0), bitmap(NULL) {}

Object::~Object() {}
