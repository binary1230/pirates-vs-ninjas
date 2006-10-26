#include "xmlParser.h"
#include "StdString.h"
#include "gameState.h"
#include "input.h"
#include "assetManager.h"
#include "window.h"
#include "gameMenu.h"
#include "globals.h"

void GameMenu::DoNewGame() {
	
	// Should load the new mode:
	// LoadNewMode(....);

	// For now, we just Let it roll onto the next mode
	GAMESTATE->SignalEndCurrentMode();
}

void GameMenu::DoQuit() {
	GAMESTATE->SignalGameExit();
}

// Hardcoded for now...
void GameMenu::DoMenuAction(const CString &action) {
	if (action == "") {
		// do nothing.
	} else if (action == "NewGame") {
		DoNewGame();
	} else if (action == "Quit") {
		DoQuit();
	} else {
		fprintf(stderr, "-- Invalid menu action: %s\n", action.c_str());
	}			
}

// #define DEBUG_VERSION_PRINT 1

void GameMenu::Draw() {
	 int x_offset = SCREEN_W/2 - back->bmp->w/2;
	 int y_offset = SCREEN_H/2 - back->bmp->h/2;

   WINDOW->DrawSprite(back, x_offset, y_offset);
	 WINDOW->DrawSprite(selector, x_offset + x_pos, y_offset + y_pos[current_pos]);
	 
	 #ifdef DEBUG_VERSION_PRINT
	 textprintf_right_ex(  WINDOW->GetDrawingSurface(), font, 
                        SCREEN_W, SCREEN_H - 10, makecol(255, 255, 255), -1, 
                        VERSION_STRING);
	 #endif
}

void GameMenu::Update() {
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
		DoMenuAction(actions[current_pos]);
	}
}

int GameMenu::Init(XMLNode xMode) {
	current_pos = 0;

	back = ASSETMANAGER->LoadSprite(xMode.getChildNode("bgPic").getText());
	if (!back) {
		fprintf(stderr, "-- MENU ERROR: Couldn't load bgPic.\n");
		return -1;
	}
	
	selector = ASSETMANAGER->LoadSprite(xMode.getChildNode("selectorPic").getText());
	if (!selector) {
		fprintf(stderr, "-- MENU ERROR: Couldn't load selectorPic\n");
		return -1;
	}

	if (!xMode.getChildNode("xpos").getInt(x_pos)) {
		fprintf(stderr, "-- MENU ERROR: Invalid xPos\n");
		return -1;	
	}

	XMLNode xPositions = xMode.getChildNode("ypositions");
	XMLNode xAction, xPos;
	int i, iterator, max = xPositions.nChildNode("ypos");
	int ypos;

	for (i=iterator=0; i < max; i++) {
		xPos = xPositions.getChildNode("ypos", &iterator);
		
		if (!xPos.getInt(ypos)) {
			fprintf(stderr, "-- MENU ERROR: Invalid yPos\n");
			return -1;
		}
		y_pos.push_back(ypos);

		if (xPos.nChildNode("action")) {
			actions.push_back(xPos.getChildNode("action").getText());
		} else {
			actions.push_back("");
		}
	}

	return 0;
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
