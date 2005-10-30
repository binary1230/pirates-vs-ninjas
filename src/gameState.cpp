#include "gameState.h"

//! Get the drawing surface (effectively the screen)
// THIS FUNCTION CAN'T BE INLINED (easily) due to dependency wack-ness
BITMAP* GameState::GetDrawingSurface() { 
	return window->GetDrawingSurface(); 
};

uint GameState::Width() {
	return window->Width();
}

uint GameState::Height() {
	return window->Height();
}

//! Initialize game systems - main function

//! This is the first init function, it needs to initialize
//! Allegro, the window, the input subsystem, and the simulation
//! BE CAREFUL, things need to be done IN ORDER here.
int GameState::InitSystem() {

		exit_game = false;

		allegro_init();
		InitTimers();

		SetRandomSeed(42);

		window = new Window();
		if ( !window ||	window->Init(this, 640, 480, 
										options->IsFullscreen()) < 0 ) {
			fprintf(stderr, "ERROR: InitSystem: failed to init window!\n");
			return -1;
		}

		if (InitInput() == -1) {
			fprintf(stderr, "ERROR: InitSystem: failed to init input subsystem!\n");
			return -1;
		}
		
		simulation = new PhysSimulation();
		if ( !simulation || simulation->Init(this) < 0) {
			fprintf(stderr, "ERROR: InitSystem: failed to init simulation!\n");
			return -1;
		}
					
		return 0;
}

//! Init input subsystems
int GameState::InitInput() {
				
	// init the right kind of class based on
	// whether or not we are recording/playing back a demo
	if ( options->RecordDemo() )
		input = new InputRecord();	
	else if ( options->PlaybackDemo() )
		input = new InputPlayback();
	else
		input = new InputLive();

	
	if ( !input || !input->Init(this, options->GetDemoFilename()) < 0 ) {
		return -1;
	}

	return 0;
}

//! Init game timers

//! This MUST be called BEFORE any other allegro initializations.
int GameState::InitTimers() {
	install_timer();
	LOCK_VARIABLE(outstanding_updates);
	LOCK_FUNCTION((void*)Timer);
	return install_int_ex(Timer, BPS_TO_TIMER(60));
}

//! Initialize game objects

//! Uses the objectFactory to create some random objects.
//int GameState::InitSimulation() {
//}

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
		while (outstanding_updates > 0) {
			Update();
			outstanding_updates--;
		}
		Draw();

		// wait for 1/60th sec to elapse (if we're on a fast computer)
		while (outstanding_updates <= 0);
  }
}

//! Update all game status

//! Call update on all objects to let them know
//! how much time has passed.
void GameState::Update() {
	input->Update();
	simulation->Update();

	if (input->Key(GAMEKEY_EXIT)) {
		exit_game = true;
	}
}

//! Draw all game objects

//! Loop through and draw all game objects.
void GameState::Draw() {
	simulation->Draw();
	window->Flip();
}

//! Shutdown the game

//! Clean up everything we allocated
void GameState::Shutdown() {

	if (simulation) {
		simulation->Shutdown();
		delete simulation;
		simulation = NULL;
	}
				
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
}

//! Destory all game objects

//! Clean up everything we allocated
//void GameState::DestroyObjects() {
//}

GameState::GameState() : 
window(NULL), input(NULL), simulation(NULL) {}
GameState::~GameState() {}
