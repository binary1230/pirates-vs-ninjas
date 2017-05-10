#include "stdafx.h"
#include "xmlParser.h"
// #include "StdString.h"
#include "game.h"
#include "input.h"
#include "assetManager.h"
#include "window.h"
#include "gameMenu.h"
#include "globals.h"

// TODO: Move this all into python scripts.

void GameMenu::DoNewGame() {

	// Should load the new mode:
	// LoadNewMode(....);

	// For now, we just Let it roll onto the next mode
	GAME->SignalEndCurrentMode();
}

void GameMenu::DoQuit() {
	GAME->SignalGameExit();
}

// Hardcoded for now...
void GameMenu::DoMenuAction(const std::string &action) {
	if (action == "") {
		// do nothing.
	}
	else if (action == "NewGame") {
		DoNewGame();
	}
	else if (action == "Quit") {
		DoQuit();
	}
	else {
		TRACE("-- Invalid menu action: %s\n", action);
	}
}

// #define DEBUG_VERSION_PRINT 1

void GameMenu::Draw() {
	int screen_w = al_get_bitmap_width(al_get_target_bitmap());
	int screen_h = al_get_bitmap_height(al_get_target_bitmap());

	int x_offset = screen_w / 2 - back->width / 2;
	int y_offset = screen_h / 2 - back->height / 2;

	int x_pos1 = x_offset + x_pos;
	int x_pos2 = screen_w / 2 + back->width / 2 - x_pos - selector->width;

	WINDOW->DrawSprite(back, x_offset, y_offset);

	int hacky_offset = 350;

	WINDOW->DrawSprite(selector,
		x_pos1 + hacky_offset,
		y_offset + y_pos[current_pos]);

	WINDOW->DrawSprite(selector,
		x_pos2 - hacky_offset,
		y_offset + y_pos[current_pos], true);

#ifdef DEBUG_VERSION_PRINT
	textprintf_right_ex(WINDOW->GetDrawingSurface(), font,
		SCREEN_W, SCREEN_H - 10, al_map_rgb(255, 255, 255), -1,
		VERSION_STRING);
#endif
}

void GameMenu::MenuPress() {
	// start animation = true
	// ...

	// when animation is finished, do this:
	DoMenuAction(actions[current_pos]);
}

void GameMenu::Update() {
	// if animation is playing..
	// UpdateAnimation();

	// else if animation is done...
	// DoMenuAction()

	// else if no animation is playing then
	CheckKeys();
}

// check and see if they pressed anthing
void GameMenu::CheckKeys() {
	if (INPUT->KeyOnce(PLAYERKEY_UP, 1)) {
		if ((current_pos--) == 0) {
			current_pos = y_pos.size() - 1;
		}
	}

	if (INPUT->KeyOnce(PLAYERKEY_DOWN, 1)) {
		if ((++current_pos) == y_pos.size())
			current_pos = 0;
	}

	if (INPUT->KeyOnce(GAMEKEY_EXIT)) {
		DoQuit();
	}

	assert(current_pos >= 0 || current_pos < y_pos.size());

	if (INPUT->KeyOnce(GAMEKEY_START) || INPUT->KeyOnce(PLAYERKEY_JUMP, 1)) {
		MenuPress();
	}
}

bool GameMenu::Init(XMLNode xMode) {
	current_pos = 0;

	back = ASSETMANAGER->LoadSprite(xMode.getChildNode("bgPic").getText());
	if (!back) {
		TRACE("-- MENU ERROR: Couldn't load bgPic.\n");
		return false;
	}

	selector = ASSETMANAGER->LoadSprite(xMode.getChildNode("selectorPic").getText());
	if (!selector) {
		TRACE("-- MENU ERROR: Couldn't load selectorPic\n");
		return false;
	}

	if (!xMode.getChildNode("xpos").getInt(x_pos)) {
		TRACE("-- MENU ERROR: Invalid xPos\n");
		return false;
	}

	XMLNode xPositions = xMode.getChildNode("ypositions");
	XMLNode xAction, xPos;
	int i, iterator, max = xPositions.nChildNode("ypos");
	int ypos;

	for (i = iterator = 0; i < max; i++) {
		xPos = xPositions.getChildNode("ypos", &iterator);

		if (!xPos.getInt(ypos)) {
			TRACE("-- MENU ERROR: Invalid yPos\n");
			return false;
		}
		y_pos.push_back(ypos);

		if (xPos.nChildNode("action")) {
			actions.push_back(xPos.getChildNode("action").getText());
		} else {
			actions.push_back("");
		}
	}

	return true;
}

void GameMenu::Shutdown() {
	back = NULL;
	selector = NULL;
	y_pos.clear();
	x_pos = 0;
	current_pos = 0;
	actions.clear();
}

GameMenu::GameMenu() {
	Shutdown();
}

GameMenu::~GameMenu() {
	Shutdown();
}
