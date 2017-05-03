#include "stdafx.h"
#include "objectController.h"

#include "assetManager.h"
#include "window.h"
#include "sprite.h"
#include "input.h"
#include "gameState.h"
#include "gameWorld.h"

int ObjectController::GetHeight() const {
	return controller_sprite->height;
}

int ObjectController::GetWidth() const {
	return controller_sprite->width;
}

void ObjectController::Draw() {

	if (only_show_during_demo && INPUT->GetInputType() != INPUT_PLAYBACK) {
		return;
	}
				
	int x = (int)_Pos.x;
	int y = (int)_Pos.y;

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

bool ObjectController::LoadObjectProperties(XMLNode &xDef) {
	if (!Object::LoadObjectProperties(xDef))
		return false;

	is_overlay = 1;

	// XXX READ which controller we monitor from XML file
	// but not in this method

	int i, iterator, max;
	std::string filename;
	XMLNode xImages, xBtn;

	xImages = xDef.getChildNode("images");
	max = xImages.nChildNode("btn");

	buttons.clear();
	buttons.resize(max);

	filename = xImages.getChildNode("base").getText();

	controller_sprite = ASSETMANAGER->LoadSprite(filename.c_str());

	if (!controller_sprite) {
		TRACE("-- ERROR: Can't load file '%s'\n", filename);
		return NULL;
	}

	int x1, y1;
	if (!xImages.getChildNode("base").getChildNode("x").getInt(x1)) {
		TRACE("Invalid controller base X!\n");
		return NULL;
	}
	if (!xImages.getChildNode("base").getChildNode("x").getInt(y1)) {
		TRACE("Invalid controller base Y!\n");
		return NULL;
	}
	_Pos.x = x1;
	_Pos.y = y1;

	Button* b;

	// load each button
	max = buttons.size();
	for (i = iterator = 0; i < max; i++) {
		xBtn = xImages.getChildNode("btn", &iterator);
		b = &buttons[i];

		filename = xBtn.getText();
		b->active = 0;

		b->sprite = ASSETMANAGER->LoadSprite(filename.c_str());

		if (!b->sprite) {
			TRACE("-- ERROR: Can't load file '%s'\n", filename);
			return NULL;
		}

		int x2, y2;
		if (!xBtn.getChildNode("x").getInt(x2)) {
			TRACE("Invalid controller button X!\n");
			return NULL;
		}
		if (!xBtn.getChildNode("y").getInt(y2)) {
			TRACE("Invalid controller button Y!\n");
			return NULL;
		}
		b->sprite->x_offset = x2;
		b->sprite->y_offset = y2;
	}

	if (xDef.nChildNode("showDuringDemoOnly") > 0)
		only_show_during_demo = true;

	return true;
}

bool ObjectController::Init() {
	buttons.clear();
	controller_sprite = NULL;
	only_show_during_demo = false;
	
	return BaseInit();
}

void ObjectController::Shutdown() {
	// Don't explicitly free stuff here
	buttons.clear();
	controller_sprite = NULL;

	BaseShutdown();
}

void ObjectController::Clear() {
	controller_sprite = NULL;
	controller_num = 1;
}

ObjectController::ObjectController() {
	Clear();
}

ObjectController::~ObjectController() {}

BOOST_CLASS_EXPORT_GUID(ObjectController, "ObjectController")