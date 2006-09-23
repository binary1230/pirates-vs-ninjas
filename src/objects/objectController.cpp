#include "objectController.h"

#include "assetManager.h"
#include "window.h"
#include "sprite.h"
#include "input.h"
#include "gameState.h"
#include "physSimulation.h"

int ControllerObject::GetHeight() {
	return controller_sprite->bmp->h;
}

int ControllerObject::GetWidth() {
	return controller_sprite->bmp->w;
}

void ControllerObject::Draw() {

	if (only_show_during_demo && INPUT->GetInputType() != INPUT_PLAYBACK) {
		return;
	}
				
	int x = (int)pos.GetX();
	int y = (int)pos.GetY();

	int bx, by;

	// Draw the base controller
	WINDOW->DrawSprite(controller_sprite, x, y);

	// Draw each button if it is active
	int i, max = buttons.size();
	for (i = 0; i < max; i++) {
		if (buttons[i].active) {
			bx = buttons[i].sprite->x_offset + x;
			by = buttons[i].sprite->y_offset + y;
			WINDOW->DrawBitmap(buttons[i].sprite->bmp, bx, by);
		}
	}
}

void ControllerObject::Update() {

	BaseUpdate();

	// keys, in the order shown on the controller
	int keys[] = {
		PLAYERKEY_LEFT,
		PLAYERKEY_RIGHT,
		PLAYERKEY_UP,
		PLAYERKEY_DOWN,
		PLAYERKEY_JUMP,
		-1
	};				

	int i, max = buttons.size();

	for (i = 0; i < max && keys[i] != -1; i++) {
		if (GAMESTATE->GetKey(keys[i], controller_num))
			buttons[i].active = 1;
		else 
			buttons[i].active = 0;
	}
}

bool ControllerObject::Init(PhysSimulation *p) {
	simulation = p;
	
	buttons.clear();
	controller_sprite = NULL;
	controller_num = 1; // we'll set this later

	only_show_during_demo = false;
	
	return BaseInit();
}

void ControllerObject::Shutdown() {
	int i, max = buttons.size();
	
	for (i = 0; i < max; i++) {
		if (buttons[i].sprite) {
			delete buttons[i].sprite;
			buttons[i].sprite = NULL;
		}
	}

	if (controller_sprite) {
		delete controller_sprite;
		controller_sprite = NULL;
	}
}

ControllerObject::ControllerObject() {
	controller_sprite = NULL;
}

ControllerObject::~ControllerObject() {}
