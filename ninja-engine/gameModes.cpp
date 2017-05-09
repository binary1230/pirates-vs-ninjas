#include "stdafx.h"
#include "ai.h"
#include "gameModes.h"
#include "xmlParser.h"
#include "assetManager.h"
#include "gameMode.h"
#include "game.h"
#include "gameWorld.h"
#include "gameMenu.h"
#include "credits.h"
#include "gameOptions.h"
#include "luaManager.h"
#include "animationeditor.h"
#include "window.h"

void GameModes::Update() {
	if (signal_game_exit)
		return;

	assert(currentMode != NULL);
	currentMode->Update();

	if (signal_end_current_mode) {
		DoEndCurrentMode();
	}

	if (signal_game_exit) {
		DoGameExit();
	}
}

void GameModes::Draw() {
	assert(currentMode != NULL);
	currentMode->Draw();
}

void GameModes::DoEndCurrentMode() {
	GameModeExitInfo exitInfo;

	signal_end_current_mode = false;

	// actually end the mode
	if (currentMode) {
		exitInfo = currentMode->GetExitInfo();
		currentMode->Shutdown();
		LUA->Clear();

		// the WORLD singleton is a special case here
		if (WORLD)
			WORLD->FreeInstance();
		else
			SAFE_DELETE(currentMode);
	}
	
	ASSETMANAGER->Free();

	if (signal_game_exit)
		return;

	std::string mode_to_load = PickNextMode(exitInfo);

	if (mode_to_load.length() == 0 || LoadMode(mode_to_load, exitInfo) == -1)
		signal_game_exit = true;
}

// Pick the next mode we should load.
// Returns an empty string if we should exit
std::string GameModes::PickNextMode(const GameModeExitInfo& exitInfo) {

	// if the exit info tells us explicitly to use a mode, then do it.
	if (exitInfo.useExitInfo && exitInfo.nextModeToLoad.length() > 0)
		return exitInfo.nextModeToLoad;

	// if exitInfo doesn't specify which mode to use, 
	// grab the next one from the master list.
		
	++currentModeIndex;
	
	// make sure we didn't run out of modes in the list
	if (currentModeIndex >= mode_files.size()) {
		return "";
	} else {
		return mode_files[currentModeIndex];
	}
}

//! Load a new mode up from the specified XML file
//! Use the specified mode exit info from the last mode that exited
//! If there was no mode exit info, just pass in a blank oldExitInfo and
//! the new mode will ignore it.
int GameModes::LoadMode(std::string mode_filename, const GameModeExitInfo& oldExitInfo) {
	currentMode = NULL;

	TRACE(" Mode Info: filename '%s'\n", mode_filename.c_str() );
	mode_filename = ASSETMANAGER->GetPathOf(mode_filename.c_str());

	if (mode_filename.empty()) {
		TRACE("ERROR: Can't load modefile named '%s'", mode_filename.c_str());
		return -1;
	}

	string modeType;
	XMLNode xMode;
	
	bool isSimulation = mode_filename.find("level_") != mode_filename.npos;

	if (isSimulation) {
		modeType = "simulation";	
	} else {
		// if we can't figure out what it is, look inside the included XML file
		xMode = XMLNode::openFileHelper(mode_filename.c_str(), "gameMode");
		modeType = xMode.getAttribute("type");
	}

	TRACE(" Mode Info: type = '%s'\n", modeType.c_str());

	if (modeType == "simulation") 
	{
		currentMode = GameWorld::CreateWorld(mode_filename);
	}
	else if (modeType == "credits") 
	{
		currentMode = new CreditsMode();
	} 
	else if (modeType == "menu") 
	{
		currentMode = new GameMenu();
	} 
	else if (modeType == "animationeditor")
	{
		currentMode = new AnimationEditorMode();
	}
	else 
	{
		currentMode = NULL;
	}

	bool error = false;

	if (!currentMode) {
		error = true;
	} else {
		// pass on the old exit info
		currentMode->SetOldExitInfo(oldExitInfo); 

		// setup the new exit info
		GameModeExitInfo exitInfo = currentMode->GetExitInfo();
		exitInfo.lastModeName = mode_filename;
		currentMode->SetExitInfo(exitInfo);
	}

	if (error || currentMode->Init(xMode) == -1) {
		TRACE("ERROR: GameModes: failed to init mode type '%s'!\n", modeType.c_str());
		return -1;
	}

	WINDOW->FadeIn(30);
		
	return 0;
}

void GameModes::DoGameExit() {
	signal_game_exit = true;
	signal_end_current_mode = true;
	GAME->SignalGameExit();
}

void GameModes::SignalEndCurrentMode() {
	signal_end_current_mode = true;

	WINDOW->FadeOut(30);
}

void GameModes::SignalGameExit() {
	signal_game_exit = true;
}

int GameModes::Init(XMLNode _xGame) {

	TRACE(" Modes: Starting init.\n");

	currentMode = NULL;
	currentModeIndex = 0;
	
	signal_game_exit = false;
	signal_end_current_mode = false;

	int i, iterator;
	int max = _xGame.nChildNode("mode_file");

	if (max <= 0) {
		TRACE(" -- Error: No modes found.\n");
		return -1;
	}

	mode_files.resize(max); 
	std::string tmp;

	// iterate backwards for push_back(), lame I know.
	for (i=iterator=0; i < max; i++) {
		mode_files[i] = _xGame.getChildNode("mode_file", &iterator).getText();
	}

	const char* firstModeToLoad = mode_files[currentModeIndex].c_str();

	// user can override the first mode's filename on the command line
	if (OPTIONS->GetFirstModeOverride()) {
		firstModeToLoad = OPTIONS->GetFirstModeOverride();
	}

	// actually load up the first mode
 	if (LoadMode(firstModeToLoad, GameModeExitInfo() ) == -1) {
		return -1;
	}

	assert(currentMode != NULL);
	
	TRACE(" Modes: Init complete.\n");

	return 0;
}

void GameModes::Shutdown() {
	signal_game_exit = true;	
	DoEndCurrentMode();
	currentModeIndex = 0;
	currentMode = NULL;
}

GameModes::GameModes() {
	currentModeIndex = 0;
	currentMode = NULL;
}

GameModes::~GameModes() {
	Shutdown();
}
