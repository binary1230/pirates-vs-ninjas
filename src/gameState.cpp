#include "gameState.h"

#include "gameOptions.h"
#include "input.h"
#include "inputLiveHandler.h"
#include "inputRecord.h"
#include "inputPlayback.h"
#include "window.h"
#include "physSimulation.h"
#include "gameMode.h"
#include "resourceLoader.h"
#include "xmlParser.h"

// XXX 	todo: mode switching code is VERY hackish and bad.
//      			need to fix that.
// 			todo: clean up, shouldn't pass XMLNode, should pass XMLNode*

// Parse the master XML file
// returns: XMLNode of first GameMode to load
XMLNode GameState::LoadXMLConfig(CString xml_filename) {
				
	// XXX currently xmlParser just DIES if it can't load 
	// the XML files or on corrupted file
	xml_filename = resourceLoader->GetPathOf(xml_filename.c_str());
	xGame = XMLNode::openFileHelper(xml_filename.c_str(), "game");
	
	XMLNode xInfo = xGame.getChildNode("info");

	// xInfo.getChildNode("map_version").getAttribute("name");
	
	fprintf(stderr, 
		" Mod Info: requires engine version '%s'\n"
		" Mod Info: map version '%s'\n"
		" Mod Info: map author '%s'\n"
		" Mod Info: Description: '%s'\n",
		xInfo.getChildNode("requires_engine_version").getText(),
		xInfo.getChildNode("game_version").getText(),
		xInfo.getChildNode("author").getText(),
		xInfo.getText() );

	// Get the filename of the XML file which contains our first mode
	CString mode_xml_filename = xGame.getChildNode("initial_mode_file").getText();
	
	fprintf(stderr, 
		" Mod Info: default map filename '%s'\n",
		mode_xml_filename.c_str());

	// Open that file, return the node
	mode_xml_filename = resourceLoader->GetPathOf(mode_xml_filename);
	XMLNode xMode = XMLNode::openFileHelper(mode_xml_filename.c_str(), "gameMode" );

	return xMode;
}

//! Initialize a "game mode" (e.g. menu, simulation, etc)
int GameState::LoadGameMode(XMLNode xMode) {
		GameMode* mode;
			
		// Get the mode type from the XML file
		CString nodeType = xMode.getAttribute("type");

		// TODO: Is it worth making a "mode factory" for this?
		if (nodeType == "simulation") {
						
			mode = physSimulation = new PhysSimulation();
			if ( !mode || mode->Init(this, xMode) < 0) {
				fprintf(stderr, "ERROR: InitSystem: failed to init simulation!\n");
				return -1;
			}
	
		} else {
			mode = NULL;
		}

		if (mode) {
			modes.push_back(mode);
			return 0;
		} else {
			return -1;
		}
}


//! Initialize game systems - main function

//! This is the first init function, it needs to initialize
//! Allegro, the window, the input subsystem, and the default game mode
//! BE CAREFUL, things need to be done IN ORDER here.
int GameState::InitSystem() {
		
		fprintf(stderr, "[Beginning Game Init]\n");
				
		exit_game = false;
		is_playing_back_demo = false;
		end_current_mode = false;

		fprintf(stderr, "[init: allegro]\n");
		allegro_init();			// must be called FIRST
		
		fprintf(stderr, "[init: timers]\n");
		InitTimers();				// must be called SECOND

		SetRandomSeed(42);
		
		fprintf(stderr, "[init: resourceLoader]\n");
		resourceLoader = new ResourceLoader();
		if (!resourceLoader || resourceLoader->Init(this) < 0) {
			fprintf(stderr, "ERROR: InitSystem: failed to create resourceLoader!\n");
			return -1;
		}

		resourceLoader->AppendToSearchPath("../");

		// just DIES if it can't load this file (bad)
		fprintf(stderr, "[init: xml config]\n");
		XMLNode xMode = LoadXMLConfig("data/default.xml");
		
		fprintf(stderr, "[init: window]\n");
		window = new Window();
		if ( !window ||	window->Init(this, SCREEN_SIZE_X, SCREEN_SIZE_Y, 
										options->IsFullscreen(), options->GraphicsMode()) < 0 ) {
			fprintf(stderr, "ERROR: InitSystem: failed to init window!\n");
			return -1;
		}

		fprintf(stderr, "[init: input subsystem]\n");
		if (InitInput() == -1) {
			fprintf(stderr, "ERROR: InitSystem: failed to init input subsystem!\n");
			return -1;
		}

		fprintf(stderr, "[init: default game mode]\n");
		if (LoadGameMode(xMode) == -1) {
			fprintf(stderr, "ERROR: InitSystem: failed to init default game mode!\n");
			return -1;
		}
		currentMode = modes[0];
		currentModeIndex = 0;
		
		fprintf(stderr, "[init complete]\n");
				
		return 0;
}

//! Init input subsystems
// a little hackish... just a bit.
int GameState::InitInput() {
				
	// init the right kind of class based on
	// whether or not we are recording/playing back a demo
	if ( options->RecordDemo() ) {
		input = new InputRecord();	
	} else if ( options->PlaybackDemo() ) {
		is_playing_back_demo = true;
		input = new InputPlayback();
	} else {
		input = new InputLive();
	}
		
	if ( !input || (input->Init(this, options->GetDemoFilename()) == -1) ) {
		return -1;
	}

	return 0;
}

//! Init game timers
//! This MUST be called BEFORE any other allegro initializations.
int GameState::InitTimers() {
	fprintf(stderr, "[Init: Timers]\n");
	install_timer();
	LOCK_VARIABLE(outstanding_updates);
	LOCK_FUNCTION((void*)Timer);
	return install_int_ex(Timer, BPS_TO_TIMER(FPS));
}

//! The 'main' function for the game

//! It takes a pointer to the game options (fullscreen/etc).
//! It initializes everything, and returns 0 if successful
//! or 1 on error.
int GameState::RunGame(GameOptions* _options) {
		
		options = _options;
		
		if (InitSystem() == -1) {
			fprintf(stderr, "ERROR: Failed to init game!\n");
			return -1;	
		} else {

			// XXX SHOULD NOT TEST option->is_xxx should TEST input->is_xxx()
			if (options->RecordDemo())
				input->BeginRecording();
			else if (options->PlaybackDemo())
				input->BeginPlayback();
			
			outstanding_updates = 0;	// reset our timer to 0.
			MainLoop();

			// XXX SHOULD NOT TEST option->is_xxx should TEST input->is_xxx()
			if (options->RecordDemo())
				input->EndRecording();
			else if (options->PlaybackDemo())
				input->EndPlayback();
		}
	
		Shutdown();

		return 0;
}

//! The Main Loop

//! The most important function.  It will make sure that the game 
//! is updating at the correct speed, and it will Draw everything
//! at the correct speed.
void GameState::MainLoop() {
				
	while (!exit_game) {

		// outstanding_updates is incremented once every 1/60th of a sec.
		// We may need to update more than once on slower computers
		// before we can draw, in order to keep the game the same speed
		// no matter the speed of the computer
		while (outstanding_updates > 0 && !exit_game) {
			Update();	// mode signals handled here
			outstanding_updates--;
		}
		if (!exit_game) Draw();

		// wait for 1/60th sec to elapse (if we're on a fast computer)
		while (outstanding_updates <= 0 && !exit_game);
  }
}

//! Update all game status
void GameState::Update() {

	// see if we were signalled
	if (end_current_mode) {
		EndCurrentMode();
		if (exit_game) 
			return;
	}
	
	input->Update();
	currentMode->Update();
}

//! Draw the current mode
void GameState::Draw() {
	window->Clear();
	currentMode->Draw();
	window->Flip();
}

//! Shutdown the game

//! Clean up everything we allocated
void GameState::Shutdown() {

	int i, max = modes.size();
	for (i = 0; i < max; i++) {
		if (modes[i]) {
			modes[i]->Shutdown();
			delete modes[i];
			modes[i] = NULL;
		}
	}

	currentMode = NULL;
				
	if (input) {
		input->Shutdown();
		delete input;
		input = NULL;
	}
		
	// window destruction code must be LAST
	if (window) {
		window->Shutdown();
		delete window;
		window = NULL;
	}
		
	allegro_exit();
	fprintf(stderr, "[Exiting]\n");	
}

//! Exits the current mode and deletes it, free its memory
//! Exits the game if it is the last mode left.

// XXX stop whining and use iterators.  this is messier than it needs to be
// XXX probably need a modeList class to handle this messiness.
// XXX should we remove currentMode->next too?  that memory will be freed
//     on shutdown() though...
void GameState::EndCurrentMode() {
	if (currentMode) {
										
		// get the prev mode (will be NULL if this is the last mode in the game)
		GameMode* parent = currentMode->GetParentMode();
					
		// kill this mode
		currentMode->Shutdown();
		currentMode = parent;
					
		// delete + remove it from the modes list
		delete modes[currentModeIndex];
		modes.erase(	modes.begin()+currentModeIndex,
									modes.begin()+currentModeIndex + 1);
	}

	// if there is no more current mode, exit the game
	if (!currentMode) {
		SignalExit();
	}

	end_current_mode = false;
}

//! Switches to the next mode if it exists. If not, does nothing.
//! Returns false if there is no next mode (usually an error)
bool GameState::SwitchToNextMode() {
	if (currentMode) {
		GameMode* next = currentMode->GetNextMode();
		if (next)
			currentMode = next;
		else 
			return false;
	}
	return true;
}

//! Switches the current mode to its parent if it exists. If it does
//! not exist, exit the game.
void GameState::SwitchToParentMode() {
	if (currentMode) {
		GameMode* parent = currentMode->GetParentMode();
		if (parent)
			currentMode = parent;
		else 
			SignalExit();
	}
}

PhysSimulation* GameState::GetPhysSimulation() {
	return physSimulation;					
}

void GameState::SetRandomSeed(int val) { 
	random_seed = val; 
	srand(val); 
};

int GameState::GetRandomSeed() const { 
	return random_seed; 
};

bool GameState::GetKey(uint which_key) const	{ 
	return input->Key(which_key); 
};

bool GameState::GetKey(uint which_key, uint which_controller) const	{ 
	return input->Key(which_key, which_controller); 
};

BITMAP* GameState::GetDrawingSurface() { 
	return window->GetDrawingSurface(); 
};

uint GameState::ScreenWidth() const {
	return window->Width();
}

uint GameState::ScreenHeight() const {
	return window->Height();
}

void GameState::SignalEndCurrentMode() {
		end_current_mode = true;
}

GameState::GameState() {
	window = NULL; 
	input = NULL;  
	currentMode = NULL; 
	physSimulation = NULL;
}

void GameState::SignalExit() {
	exit_game = true; 
	is_playing_back_demo = false;
}

GameState::~GameState() {}
