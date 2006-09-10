#ifndef GAMEMODES_H
#define GAMEMODES_H

#include "stdafx.h"
#include "gameBase.h"

class GameMode;

class GameModes : public GameBase {
	protected:
		unsigned int currentModeIndex;

		bool signal_end_current_mode;
		bool signal_game_exit;

		vector<CString> mode_files;

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

		// experimental, called at the end of each mode to do
		// AI training stuff.
		void DoAIEndStuff();

		GameModes();
		~GameModes();
};

#endif // GAMEMODES_H
