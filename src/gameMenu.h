#ifndef GAME_MENU_H
#define GAME_MENU_H

// Note: This class is currently PLACEHOLDER for real menu and UI stuff.

#include <allegro.h>
#include <vector>
using namespace std;

#include "gameMode.h"
#include "StdString.h"

class GameMenu : public GameMode {
	protected:
		BITMAP *back, *selector;
		vector<int> y_pos;
		int x_pos;

		uint current_pos;

		vector<CString> actions;

		void DoNewGame();
		void DoQuit();
		void DoMenuAction(CString action);
		
	public:
		void Update();

		void Draw();

		//! Init the sound system
		int Init(GameState* _game_state, XMLNode xMode);

		//! Shutdown the sound system
		void Shutdown();

		GameMenu();
		~GameMenu();
};

#endif // GAME_MENU_H
