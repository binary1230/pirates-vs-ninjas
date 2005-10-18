#include "objectMouseBlock.h"

void MouseBlockObject::Update() {
	x = mouse_x;
	y = mouse_y;
}

void MouseBlockObject::Draw() {
	draw_sprite_h_flip(dst_bitmap, bitmap, x, y);
}

MouseBlockObject::MouseBlockObject() {}
MouseBlockObject::~MouseBlockObject() {}
