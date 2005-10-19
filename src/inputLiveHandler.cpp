#include "inputLiveHandler.h"

int InputLive::Init() {
	/*if (	install_mouse() 		== 0	&& 
				install_keyboard() 	== 0	)
		return 0;
	else
		return -1;*/
	install_mouse();
	install_keyboard();

	return 0;
}

void InputLive::Shutdown() {
	remove_mouse();
	remove_keyboard();
}

bool InputLive::Key(unsigned char index) {
	return key[index];
}

int InputLive::MouseX() {
	return mouse_x;
}

int InputLive::MouseY() {
	return mouse_y;
}

InputLive::InputLive() {}
InputLive::~InputLive() {}
