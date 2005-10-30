#include "object.h"

// Putting this here is a nasty hack to avoid weird dependency loops
#include "gameState.h"

int Object::GetX() {
	return x;
}

int Object::GetY() {
	return y;
}

void Object::SetX(int _x) {
	x = _x;
}

void Object::SetY(int _y) {
	y = _y;
}

void Object::SetXY(int _x, int _y) {
	x = _x;
	y = _y;
}

int Object::GetWidth() {
	return bitmap->w;
}

int Object::GetHeight() {
	return bitmap->h;
}

void Object::Draw() {
	DrawAtOffset(0,0);
}

void Object::DrawAtOffset(int _x, int _y) {	
	draw_sprite(game_state->GetDrawingSurface(), bitmap, x + _x, y + _y);
}

void Object::Update() {
	// nothing to update for regular objects
}

void Object::SetBitmap(BITMAP* _bitmap) {
	bitmap = _bitmap;
}
				
void Object::SetBitmapIsDeleteable(bool _bitmap_is_deleteable) {
	bitmap_is_deleteable = _bitmap_is_deleteable;
}

bool Object::GetBitmapIsDeleteable() {
	return bitmap_is_deleteable;
}

void Object::Shutdown() {
	if (bitmap_is_deleteable) 
		destroy_bitmap(bitmap);

	bitmap = NULL;
}

Object::Object() : 
x(0), y(0), bitmap(NULL), bitmap_is_deleteable(false)  {}
Object::~Object() {}
