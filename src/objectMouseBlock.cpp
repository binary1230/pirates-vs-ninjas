#include "objectMouseBlock.h"

void MouseBlockObject::Update() {
	x = mouse_x;
	y = mouse_y;
}

void MouseBlockObject::Draw() {
	DrawAtOffset(0,0);
}

MouseBlockObject::MouseBlockObject() {}
MouseBlockObject::~MouseBlockObject() {}
