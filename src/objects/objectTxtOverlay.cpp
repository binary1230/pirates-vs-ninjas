#include "objectTxtOverlay.h"

#include "assetManager.h"
#include "window.h"
#include "sprite.h"
#include "input.h"
#include "gameState.h"
#include "physSimulation.h"

int ObjectText::GetHeight() {
	return 0;
}

int ObjectText::GetWidth() {
	return 0;
}

void ObjectText::Draw() {

	int x = (int)pos.GetX();
	int y = (int)pos.GetY();

	/*
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
	}*/
}

void ObjectText::Update() {
	BaseUpdate();

}

bool ObjectText::Init(PhysSimulation *p) {
	simulation = p;
	
	return BaseInit();
}

void ObjectText::Shutdown() {

}

ObjectText::ObjectText() {
}

ObjectText::~ObjectText() {}
