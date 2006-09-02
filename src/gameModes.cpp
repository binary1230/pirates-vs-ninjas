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
	AssetManager* m = GetGameState()->GetAssetManager();
	m->Free();

	signal_end_current_mode = false;
	currentModeIndex++;

	if (currentMode) {
		currentMode->Shutdown();
		free(currentMode);
		currentMode = NULL;
	}

	// went through all the modes, now exit.
	if (currentModeIndex == mode_files.size()) {
		signal_game_exit = true;
		return;
	}

	if (!signal_game_exit) {
		if (LoadNextMode() == -1) 
			DoGameExit();
	} else {
		DoGameExit();
	}
}

void GameModes::DoGameExit() {
	signal_game_exit = true;
	signal_end_current_mode = true;
	GetGameState()->SignalGameExit();
}

void GameModes::SignalEndCurrentMode() {
	signal_end_current_mode = true;
}

void GameModes::SignalGameExit() {
	signal_game_exit = true;
}

int GameModes::Init(GameState* gs, XMLNode _xGame) {

	fprintf(stderr, " Modes: Starting init.\n");

	SetGameState(gs);
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

 	if (LoadNextMode() == -1) {
		return -1;
	}

	if (!currentMode)
	assert(currentMode);
	
	fprintf(stderr, " Modes: Init complete.\n");

	return 0;
}

int GameModes::LoadNextMode() {
	AssetManager* m = GetGameState()->GetAssetManager();
	currentMode = NULL;

	// Get the mode type from the XML file
	assert(mode_files.size() > 0);
	
	CString mode_xml_filename = mode_files[currentModeIndex];

	fprintf(stderr, 
    " Mod Info: default mode filename '%s'\n",
    mode_xml_filename.c_str());

	mode_xml_filename = m->GetPathOf(mode_xml_filename);
	XMLNode xMode = XMLNode::openFileHelper(mode_xml_filename.c_str(), "gameMode");
	CString nodeType = xMode.getAttribute("type");
	
	fprintf(stderr, " Mod Info: type = '%s'\n", nodeType.c_str());


	if (nodeType == "simulation") {
						
		currentMode = new PhysSimulation();
		if ( !currentMode || currentMode->Init(GetGameState(), xMode) == -1) {
			fprintf(stderr, "ERROR: GameModes: failed to init simulation!\n");
			return -1;
			fprintf(stderr, "CRAP!\n");
		}

	}	else if (nodeType == "credits") {
						
		currentMode = new CreditsMode();
		if ( !currentMode || currentMode->Init(GetGameState(), xMode) < 0) {
			fprintf(stderr, "ERROR: GameModes: failed to init simulation!\n");
			return -1;
		}
	
	} else if (nodeType == "menu") {

		currentMode = new GameMenu();
		if ( !currentMode || currentMode->Init(GetGameState(), xMode) < 0) {
			fprintf(stderr, "ERROR: GameModes: failed to init menu!\n");
			return -1;
		}
		
	} else {
		currentMode = NULL;
	}

	if (currentMode) {
		return true;
	} else {
		fprintf(stderr, " ERROR: No current mode!.\n");
		return false;
	}
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
