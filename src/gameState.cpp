#include "gameState.h"

#include "loadpng.h"
#include "gameOptions.h"
#include "input.h"
#include "inputLiveHandler.h"
#include "inputRecord.h"
#include "inputPlayback.h"
#include "window.h"
#include "gameMode.h"
#include "assetManager.h"
#include "xmlParser.h"
#include "gameSound.h"
#include "gameModes.h"
#include "network.h"

// Parse the master XML file
// returns: XMLNode of first GameMode to load
int GameState::LoadXMLConfig(CString xml_filename) {
				
	int default_mode_id = options->GetDefaultModeId();

	// XXX xmlParser just DIES on error
	xml_filename = assetManager->GetPathOf(xml_filename.c_str());
	xGame = XMLNode::openFileHelper(xml_filename.c_str(), "game");
	
	XMLNode xInfo = xGame.getChildNode("info");

	fprintf(stderr, 
		" Mod Info: requires engine version '%s'\n"
		" Mod Info: map version '%s'\n"
		" Mod Info: map author '%s'\n"
		" Mod Info: Description: '%s'\n"
		// " Mod Info: Number of modes: '%i'\n"
		" Mod Info: Default Mode ID: '%i'\n",
		xInfo.getChildNode("requires_engine_version").getText(),
		xInfo.getChildNode("game_version").getText(),
		xInfo.getChildNode("author").getText(),
		xInfo.getText(),
		//max, 
		default_mode_id);

	return 0;	
}

void GameState::SignalEndCurrentMode() {
	modes->SignalEndCurrentMode();
}

//! Initialize basic allegro library stuff
//! This must be called FIRST before ANY allegro stuff
int GameState::InitAllegro() {
	
	if (allegro_init() != 0) {
		fprintf(stderr, "-- FATAL ERROR: Allegro_init() failed.\n");
		return -1;
	}
	
	// must be called SECOND
	if (InitTimers() < 0) {
		fprintf(stderr, "-- FATAL ERROR: Can't init timers.\n");
		return -1;
	}

	// register PNG as allegro filetype (see loadpng.h)
	register_png_file_type();

	SetRandomSeed(42);	// for now, makes testing easier

	return 0;
}

//! Initialize game systems - main function

//! This is the first init function, it needs to initialize
//! Allegro, the window, the input subsystem, and the default game mode
//! BE CAREFUL, things need to be done IN ORDER here.
int GameState::InitSystem() {
		
		fprintf(stderr, "[Beginning Game Init]\n");
				
		exit_game = false;
		is_playing_back_demo = false;
		debug_pause_toggle = false;

		fprintf(stderr, "[init: allegro]\n");
		if (InitAllegro() < 0) {
			fprintf(stderr, "ERROR: InitSystem: failed to init allegro!\n");
			return -1;
		}

		fprintf(stderr, "[init: assetManager]\n");
		assetManager = new AssetManager();
		if (!assetManager || assetManager->Init(this) < 0) {
			fprintf(stderr, "ERROR: InitSystem: failed to create assetManager!\n");
			return -1;
		}

		assetManager->AppendToSearchPath("data/");

		fprintf(stderr, "[init: xml config]\n");

		// just DIES if it can't load this file (bad)
		if (LoadXMLConfig("default.xml") < 0) {
			fprintf(stderr, "ERROR: Failed to parse default.xml");	
			return -1;
		}
		
		fprintf(stderr, "[init: window]\n");
		window = new Window();
		if ( !window ||	window->Init(this, screen_size_x, screen_size_y, 
										options->IsFullscreen(), options->GraphicsMode()) < 0 ) {
			fprintf(stderr, "ERROR: InitSystem: failed to init window!\n");
			return -1;
		}

		if (InitNetwork() == -1) {
			fprintf(stderr, "ERROR: InitSystem: failed to init network!\n");
			return -1;
		}
	
		fprintf(stderr, "[init: input subsystem]\n");
		if (InitInput() == -1) {
			fprintf(stderr, "ERROR: InitSystem: failed to init input subsystem!\n");
			return -1;
		}

		fprintf(stderr, "[init: sound subsystem]\n");
		if (InitSound() == -1) {
			fprintf(stderr, "ERROR: InitSystem: failed to init sound subsystem!\n");
		}

		fprintf(stderr, "[init: loading game modes]\n");
		if (LoadGameModes() == -1) {
			fprintf(stderr, "ERROR: InitSystem: failed to init default game mode!\n");
			return -1;
		}
		
		fprintf(stderr, "[init complete]\n");
				
		return 0;
}

int GameState::LoadGameModes() {
	modes = new GameModes();

	if (!modes)
		return -1;

	return modes->Init(this, xGame);
}
 
/*#define PVN_NETWORK_MAGIC_GREETING 123454321

int GameState::InitNetworkServer() {
	int port = options->GetNetworkPortNumber();
  ezSocketsPacket packet;
	
	fprintf(stderr, "NET: Starting UDP network server on port %i\n", port);
	
  socket->mode = ezSockets::skUDP;
  socket->Create(IPPROTO_UDP, SOCK_DGRAM);
  socket->Bind(port);
	
	fprintf(stderr, "NET: Waiting for client greeting..\n");

	bool got_greeting = false;

	while (!got_greeting) {
		rest(100);
		if (socket->ReadPack(packet)) {
			int size = packet.Read4();
      if (size != packet.Size-4)
        fprintf(stderr, "NET: WARN: Merged packets!\n");

			// Expect MAGIC greeting from client
			if (packet.Read4() != PVN_NETWORK_MAGIC_GREETING) {
				fprintf(stderr, "Incorrect MAGIC recieved from client, aborting!\n");
				return -1;	
			} else {
				got_greeting = true;
			}
    }
	}

	rest(1000);
	packet.ClearPacket();

	// Send MAGIC greeting to client
  packet.Write4(PVN_NETWORK_MAGIC_GREETING);
	socket->SendPack(packet);

	fprintf(stderr, "NET: Server: Connected to client OK!\n");

	return 0;
}*/

int GameState::InitNetwork() {	
	int ret;
	int port = options->GetNetworkPortNumber();

	if (!options->IsNetworkEnabled())
		return 0;

	network = new GameNetwork();
	if (!network)
		return -1;

	fprintf(stderr, "NET: Port %i\n", port);
	
	if (options->IsNetworkServer()) {
		ret = network->InitServer(this, port);
	} else {
		ret = network->InitClient(this, port, options->GetNetworkServerName());
	}

	return ret;
}

//! Init sound subsystem
//TODO if sound init fails, make it just keep going instead of erroring out.
int GameState::InitSound() {

	sound = new GameSound();

	if (!options->SoundEnabled())
		fprintf(stderr, " Sound disabled.\n");

	if ( !sound || (sound->Init(this, options->SoundEnabled()) == -1) ) {
		return -1;
	}
				
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

			if (options->GetDebugStartPaused()) 
				debug_pause_toggle = 1;	

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
		fprintf(stderr, "[Exiting]\n");	

		return 0;
}

//! The Main Loop

//! The most important function.  It will make sure that the game 
//! is updating at the correct speed, and it will Draw everything
//! at the correct speed.

// XXX NOTE: This is currently a bit more complex than it should be.
// If you are trying to understand it, ignore all the DEBUG_ and pause
// junk.  The whole release_ thing needs to be refactored into the INPUT
// class as well.
void GameState::MainLoop() {

	bool wait_for_updates = options->WaitForUpdates();
				
	int debug_update_count = 0;

	while (!exit_game) {

		// outstanding_updates is incremented once every 1/60th of a sec.
		// We may need to update more than once on slower computers
		// before we can draw, in order to keep the game the same speed
		// no matter the speed of the computer
		while (outstanding_updates > 0 && !exit_game) {
			Update();	// mode signals handled here

			if (input->KeyOnce(GAMEKEY_DEBUGPAUSE))
				debug_pause_toggle = !debug_pause_toggle;

			if (debug_pause_toggle) {
			
				debug_update_count = outstanding_updates;

				while (debug_pause_toggle && !input->KeyOnce(GAMEKEY_DEBUGSTEP)) {
					input->Update();
					Draw();

					if (input->KeyOnce(GAMEKEY_SCREENSHOT))
						window->Screenshot();

					if (input->KeyOnce(GAMEKEY_DEBUGPAUSE))
						debug_pause_toggle = !debug_pause_toggle;
				}

				outstanding_updates = debug_update_count;
			}

			outstanding_updates--;
		}

		if (!exit_game) {
			Draw();

			if (input->KeyOnce(GAMEKEY_SCREENSHOT))
				window->Screenshot();
		}

		// wait for 1/60th sec to elapse (if we're on a fast computer)
		// note: this should really be down() on a lock of some kind rather than
		// just sleep randomly.
		while (wait_for_updates && outstanding_updates <= 0 && !exit_game) {
			// rest(10);	// 1/30 sec is 33 usec, we sleep for 10
		}
  }
}

//! Update all game status
void GameState::Update() {

	if (exit_game)
		return;

	input->Update();
	modes->Update();
}

//! Draw the current mode
void GameState::Draw() {
	window->Clear();
	modes->Draw();
	window->Flip();
}

void GameState::Shutdown() {
	fprintf(stderr, "[Shutting Down]\n");	

	if (input) {
		input->Shutdown();
		delete input;
	}

	if (network) {
		network->Shutdown();
		delete network;
	}

	if (modes) {
		modes->Shutdown();
		delete modes;
	}

	if (sound) {
		sound->Shutdown();
		delete sound;
	}

	if (assetManager) {
		assetManager->Shutdown();
		delete assetManager;
	}
	
	// window destruction code must be LAST
	if (window) {
		window->Shutdown();
		delete window;
	}

	options = NULL;
	assetManager = NULL;
	modes = NULL;
	window = NULL; 
	input = NULL;  
	sound = NULL;
	network = NULL;
		
	allegro_exit();
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

GameState::GameState() {
	options = NULL;
	assetManager = NULL;
	modes = NULL;
	window = NULL; 
	input = NULL;  
	sound = NULL;
	network = NULL;
}

void GameState::SignalGameExit() {
	exit_game = true; 
	is_playing_back_demo = false;
	modes->SignalGameExit();
}

GameState::~GameState() {}
