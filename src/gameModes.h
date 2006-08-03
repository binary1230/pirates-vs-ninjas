#ifndef GAMEMODES_H
#define GAMEMODES_H

#include "gameBase.h"
#include "xmlParser.h"
#include "StdString.h"

class GameMode;

#include <vector>
using namespace std;

class GameModes : public GameBase {
	protected:
		unsigned int currentModeIndex;

		bool signal_end_current_mode;
		bool signal_game_exit;

		vector<CString> mode_files;

		GameMode* currentMode;

		void DoEndCurrentMode();
		void DoGameExit();

		bool LoadNextMode();

	public:
		void Update();
		void Draw();

		//! Should point to a <game> element with <mode_file> tags
		int Init(GameState* gs, XMLNode _xGame);
		void Shutdown();

		void SignalEndCurrentMode();
		void SignalGameExit();

		void LoadMode(int which);

		GameModes();
		~GameModes();
};

#endif // GAMEMODES_H
