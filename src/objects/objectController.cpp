#include "stdafx.h"
#include "objectController.h"

#include "assetManager.h"
#include "window.h"
#include "sprite.h"
#include "input.h"
#include "gameState.h"
#include "gameWorld.h"

int ObjectController::GetHeight() {
	return controller_sprite->height;
}

int ObjectController::GetWidth() {
	return controller_sprite->width;
}

void ObjectController::Draw() {

	if (only_show_during_demo && INPUT->GetInputType() != INPUT_PLAYBACK) {
		return;
	}
				
	int x = (int)pos.x;
	int y = (int)pos.y;

	int bx, by;

	// Draw the base controller
	WINDOW->DrawSprite(controller_sprite, x, y);

	// Draw each button if it is active
	int i, max = buttons.size();
	for (i = 0; i < max; i++) {
		if (buttons[i].active) {
			bx = buttons[i].sprite->x_offset + x;
			by = buttons[i].sprite->y_offset + y;
			WINDOW->DrawSprite(buttons[i].sprite, x, y);
		}
	}
}

void ObjectController::Update() {

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
		if (INPUT->Key(keys[i], controller_num))
			buttons[i].active = 1;
		else 
			buttons[i].active = 0;
	}
}

bool ObjectController::Init() {
	buttons.clear();
	controller_sprite = NULL;
	controller_num = 1; // we'll set this later

	only_show_during_demo = false;
	
	return BaseInit();
}

void ObjectController::Shutdown() {
	// Don't explicitly free stuff here
	buttons.clear();
	controller_sprite = NULL;

	BaseShutdown();
}

ObjectController::ObjectController() {
	controller_sprite = NULL;
}

ObjectController::~ObjectController() {}

BOOST_CLASS_EXPORT_GUID(ObjectController, "ObjectController")