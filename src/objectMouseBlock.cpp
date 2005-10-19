#include "objectMouseBlock.h"

void MouseBlockObject::SetInputHandler(BaseInput* handler) {
	input = handler;
}

void MouseBlockObject::Update() {	
	//x = input->MouseX();
	//y = input->MouseY();

	x = vectors[_X].CalcNextStep();
	y = -vectors[_Y].CalcNextStep();

	if (y > dst_bitmap->h - bitmap->h) {
			y = dst_bitmap->h - bitmap->h;
	}

	if (input->Key(KEY_SPACE)) {
			vectors[_Y].velocity = 10;
	}
}

void MouseBlockObject::Draw() {
	draw_sprite_h_flip(dst_bitmap, bitmap, x, y);
}

void MouseBlockObject::SetXY(int _x, int _y) {
	x = _x; 
	y = _y;
	vectors[_Y].position = (float)y;
	vectors[_X].position = (float)x;
	
printf("definiately setting them!!\n");
}

MouseBlockObject::MouseBlockObject() : input(NULL) {
	vectors[_X].Clear();
	vectors[_Y].Clear();
	
	// Force* f = new ForceGravity();
	vectors[_Y].Attach(new ForceGravity());
}

MouseBlockObject::~MouseBlockObject() {}
