#include "objectMouseBlock.h"

void MouseBlockObject::SetInputHandler(BaseInput* handler) {
	input = handler;
}

void MouseBlockObject::Update() {
	x = input->MouseX();
	y = input->MouseY();
	//x = mouse_x;
	//y = mouse_y;
}

void MouseBlockObject::Draw() {
	draw_sprite_h_flip(dst_bitmap, bitmap, x, y);
}

MouseBlockObject::MouseBlockObject() : input(NULL) {}
MouseBlockObject::~MouseBlockObject() {}
