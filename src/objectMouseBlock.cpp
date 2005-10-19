#include "objectMouseBlock.h"

void MouseBlockObject::SetInputHandler(BaseInput* handler) {
	input = handler;
}

void MouseBlockObject::Update() {
	x = input->MouseX();
	y = input->MouseY();
}

void MouseBlockObject::Draw() {
	draw_sprite_h_flip(dst_bitmap, bitmap, x, y);
}

MouseBlockObject::MouseBlockObject() : input(NULL) {}
MouseBlockObject::~MouseBlockObject() {}
