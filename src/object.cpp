#include "object.h"

int Object::GetX() {
	return x;
}

int Object::GetY() {
	return y;
}

void Object::Draw() {
	DrawAtOffset(0,0);
}

void Object::DrawAtOffset(int _x, int _y) {	
	draw_sprite(dst_bitmap, bitmap, x + _x, y + _y);
}

void Object::Update() {
	// nothing to update for regular objects
}

void Object::SetDestinationBitmap(BITMAP* _dst_bitmap) {
	dst_bitmap = _dst_bitmap;
}

void Object::SetBitmap(BITMAP* _bitmap) {
	bitmap = _bitmap;
}
				
Object::Object() : x(0), y(0), bitmap(NULL) {}
Object::~Object() {}
