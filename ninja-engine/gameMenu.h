#ifndef GAME_MENU_H
#define GAME_MENU_H

// Note: This class is currently PLACEHOLDER for real menu and UI stuff.

#include "gameMode.h"
#include "sprite.h"

class GameMenu : public GameMode {
	protected:
		Sprite *back, *selector;
		vector<int> y_pos;
		int x_pos;

		uint current_pos;

		vector<std::string> actions;

		void DoNewGame();
		void DoQuit();
		void DoMenuAction(const std::string &action);

		void MenuPress();
		void CheckKeys();
		
	public:
		void Update();

		void Draw();

		bool Init(XMLNode xMode);

		//! Shutdown the sound system
		void Shutdown();

		GameMenu();
		~GameMenu();
};

#endif // GAME_MENU_H
