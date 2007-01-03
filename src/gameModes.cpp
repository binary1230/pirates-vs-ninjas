#include "ai.h"
#include "gameModes.h"
#include "xmlParser.h"
#include "StdString.h"
#include "assetManager.h"
#include "gameMode.h"
#include "gameState.h"
#include "physSimulation.h"
#include "gameMenu.h"
#include "credits.h"

void GameModes::Update() {
	if (signal_game_exit)
		return;

	assert(currentMode);
	
	currentMode->Update();

	if (signal_end_current_mode) {
		DoEndCurrentMode();
	}

	if (signal_game_exit) {
		DoGameExit();
	}
}

void GameModes::Draw() {
	assert(currentMode);
	currentMode->Draw();
}

void GameModes::DoEndCurrentMode() {
	GameModeExitInfo exitInfo;

	signal_end_current_mode = false;

	// actually end the mode
	if (currentMode) {
		DoAIEndStuff();
		exitInfo = currentMode->GetExitInfo();
		currentMode->Shutdown();
		SAFE_DELETE(currentMode);
	}
	
	ASSETMANAGER->Free();

	if (signal_game_exit)
		return;

	CString mode_to_load = PickNextMode(exitInfo);

	if (mode_to_load.size() == 0 || LoadMode(mode_to_load, exitInfo) == -1)
		signal_game_exit = true;
}

// Pick the next mode we should load.
// Returns an empty string if we should exit
CString GameModes::PickNextMode(const GameModeExitInfo& exitInfo) {

	// if the exit info tells us explicitly to use a mode, then do it.
	if (exitInfo.useExitInfo && exitInfo.nextModeToLoad.size() > 0)
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
int GameModes::LoadMode(	CString mode_xml_filename, 
													const GameModeExitInfo& oldExitInfo ) {
	currentMode = NULL;

	#ifdef AI_TRAINING
	fprintf(stderr, " AI: Enabling AI Training.\n");
	#endif

	fprintf(stderr, " Mode Info: filename '%s'\n",
							    mode_xml_filename.c_str() );

	mode_xml_filename = ASSETMANAGER->GetPathOf(mode_xml_filename);
	XMLNode xMode = XMLNode::openFileHelper(	mode_xml_filename.c_str(), 
																						"gameMode"	);

	CString modeType = xMode.getAttribute("type");
	fprintf(stderr, " Mode Info: type = '%s'\n", modeType.c_str());

	// actually create the new mode
	if (modeType == "simulation")
		currentMode = new PhysSimulation();
	else if (modeType == "credits")
		currentMode = new CreditsMode();
	else if (modeType == "menu")
		currentMode = new GameMenu();
	else
		currentMode = NULL;

	bool error = false;

	if (!currentMode) {
		error = true;
	} else {
		// pass on the old exit info
		currentMode->SetOldExitInfo(oldExitInfo); 

		// setup the new exit info
		GameModeExitInfo exitInfo = currentMode->GetExitInfo();
		exitInfo.lastModeName = mode_xml_filename;
		currentMode->SetExitInfo(exitInfo);
	}

	if (error || currentMode->Init(xMode) == -1) {
			fprintf(stderr, "ERROR: GameModes: failed to init mode type '%s'!\n",
											modeType.c_str());
			return -1;
	}
		
	return 0;
}

void GameModes::DoGameExit() {
	signal_game_exit = true;
	signal_end_current_mode = true;
	GAMESTATE->SignalGameExit();
}

void GameModes::SignalEndCurrentMode() {
	signal_end_current_mode = true;
}

void GameModes::SignalGameExit() {
	signal_game_exit = true;
}

int GameModes::Init(XMLNode _xGame) {

	fprintf(stderr, " Modes: Starting init.\n");

	currentMode = NULL;
	currentModeIndex = 0;
	
	signal_game_exit = false;
	signal_end_current_mode = false;

	int i, iterator;
	int max = _xGame.nChildNode("mode_file");

	if (max <= 0) {
		fprintf(stderr, " -- Error: No modes found.\n");
		return -1;
	}

	mode_files.resize(max); 
	CString tmp;

	// iterate backwards for push_back(), lame I know.
	for (i=iterator=0; i < max; i++) {
		mode_files[i] = _xGame.getChildNode("mode_file", &iterator).getText();
	}

	// actually load up the first mode
 	if (LoadMode(mode_files[currentModeIndex], GameModeExitInfo() ) == -1) {
		return -1;
	}

	assert(currentMode != NULL);
	
	fprintf(stderr, " Modes: Init complete.\n");

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

// ------ EXPERIMENTAL AI STUFF ------

#ifndef AI_TRAINING
void GameModes::DoAIEndStuff() {}
#else

#define AI_RESULTS_FILE "AI-results.txt"

// experimental: compute ending AI stuff for this mode
void GameModes::DoAIEndStuff() {

	// output the fitness score
	int score = currentMode->GetAiFitnessScore();

	// print it to a file
	fprintf(stderr, "AI: Writing end-game fitness score to file '" 
									AI_RESULTS_FILE "'.\n");

	FILE* f = fopen(AI_RESULTS_FILE, "w");
	if (!f) {
		fprintf(stderr, "AI: ERROR: Can't open AI results file for writing!\n");
		return;
	}

	fprintf(f, "%i\n", score);

	fclose(f);
}

#endif // AI_TRAINING
