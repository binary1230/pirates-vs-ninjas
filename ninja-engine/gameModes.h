#ifndef GAMEMODES_H
#define GAMEMODES_H

class GameMode;
struct GameModeExitInfo;

class GameModes {
	protected:
		unsigned int currentModeIndex;

		bool signal_end_current_mode;
		bool signal_game_exit;

		vector<std::string> mode_files;

		GameMode* currentMode;

		void DoEndCurrentMode();
		void DoGameExit();

		bool LoadMode(std::string, const GameModeExitInfo& exitInfo);
		std::string PickNextMode(const GameModeExitInfo& exitInfo);

	public:
		void Update();
		void Draw();

		//! Should point to a <game> element with <mode_file> tags
		bool Init(XMLNode _xGame);
		void Shutdown();

		void SignalEndCurrentMode();
		void SignalGameExit();

		GameModes();
		~GameModes();
};

#endif // GAMEMODES_H
