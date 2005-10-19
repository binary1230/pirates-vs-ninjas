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

			// XXX remember affecting BOTH int y and float vectors[1].y
			y = dst_bitmap->h - bitmap->h;
			vectors[_Y].position = -y;
	}

	// if we're OK to jump now.. do it.
	if (y == dst_bitmap->h - bitmap->h) {
		if (input->Key(KEY_SPACE)) {
			vectors[_Y].velocity = Rand(2,12);
	  }		
	}

	if (x < 0) {
			vectors[_X].position = x = 0;
			vectors[_X].velocity = -vectors[_X].velocity;
	}
	
	if (x > dst_bitmap->w - bitmap->w) {
			vectors[_X].position = x = dst_bitmap->w - bitmap->w;
			vectors[_X].velocity = -vectors[_X].velocity;
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
	
	vectors[_X].Attach(new ForceInput());
	vectors[_Y].Attach(new ForceGravity());
}

MouseBlockObject::~MouseBlockObject() {}
