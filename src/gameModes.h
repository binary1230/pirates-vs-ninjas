#ifndef GAMEMODES_H
#define GAMEMODES_H

#include "gameBase.h"
#include "xmlParser.h"
#include "StdString.h"

class GameMode;

#include "stl_wrapper.h"

class GameModes : public GameBase {
	protected:
		unsigned int currentModeIndex;

		bool signal_end_current_mode;
		bool signal_game_exit;

		std::vector<CString> mode_files;

		GameMode* currentMode;

		void DoEndCurrentMode();
		void DoGameExit();

		int LoadNextMode();

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
