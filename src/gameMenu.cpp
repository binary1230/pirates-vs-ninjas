#include "xmlParser.h"
#include "StdString.h"
#include "gameState.h"
#include "input.h"
#include "assetManager.h"
#include "window.h"
#include "gameMenu.h"
#include "gameBase.h"
#include "globals.h"

void GameMenu::DoNewGame() {
	
	// Load the new mode:
	// LoadNewMode(....);

	// Let it roll.
	GetGameState()->SignalEndCurrentMode();
}

void GameMenu::DoQuit() {
	GetGameState()->SignalEndCurrentMode();
}

// Hardcoded for now...
void GameMenu::DoMenuAction(CString action) {
	if (action == "") {
		// do nothing.
	} else if (action == "NewGame") {
		DoNewGame();
	} else if (action == "Quit") {
		DoQuit();
	} else {
		fprintf(stderr, "-- Invalid menu action: %s\n");
	}			
}

#define DEBUG_VERSION_PRINT 1

void GameMenu::Draw() {
	 Window* window = GetGameState()->GetWindow();

   window->DrawBitmap(back, 0, 0);
	 window->DrawBitmap(selector, x_pos, y_pos[current_pos]);
	 
	 #ifdef DEBUG_VERSION_PRINT
	 textprintf_right_ex(  window->GetDrawingSurface(), font, 
                        SCREEN_W, SCREEN_H - 10, makecol(255, 255, 255), -1, 
                        VERSION_STRING);
	 #endif
}

void GameMenu::Update() {
	BaseInput* input = GetGameState()->GetInput();

	if (input->KeyOnce(PLAYERKEY_UP, 1)) {
		if ((current_pos--) == 0) {
			current_pos = y_pos.size() - 1;
		}
  }

	if (input->KeyOnce(PLAYERKEY_DOWN, 1)) {
		if ((++current_pos) == y_pos.size())
			current_pos = 0;
	}
	
	if (input->KeyOnce(GAMEKEY_EXIT)) {
    DoQuit();
  }
	
	assert(current_pos >= 0 || current_pos < y_pos.size());

	if (input->KeyOnce(GAMEKEY_START)) {
		DoMenuAction(actions[current_pos]);
	}
}

int GameMenu::Init(GameState* gs, XMLNode xMode) {
	SetGameState(gs);
	AssetManager* m = GetGameState()->GetAssetManager();
	
	current_pos = 0;

	back = m->LoadBitmap(xMode.getChildNode("bgPic").getText());
	if (!back) {
		fprintf(stderr, "-- MENU ERROR: Couldn't load bgPic.\n");
		return -1;
	}
	
	selector = m->LoadBitmap(xMode.getChildNode("selectorPic").getText());
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
