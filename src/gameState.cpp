#include "gameState.h"

//! Get the drawing surface (effectively the screen)
// THIS FUNCTION CAN'T BE INLINED (easily)
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
//! Allegro, the window, the input subsystem, and the object
//! factory.
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
		}
			
		objectFactory = new ObjectFactory();
		if ( !objectFactory || objectFactory->Init(this) < 0 ) {
			fprintf(stderr, "ERROR: InitSystem: failed to init objectFactory!\n");
			return -1;
		}
					
		return 0;
}

//! Init input subsystems
int GameState::InitInput() {
				
	if ( options->RecordDemo() )
		input = new InputRecord();
	
	else if ( options->PlaybackDemo() )
		input = new InputPlayback();
	
	else
		input = new InputLive();

	if ( !input || !input->Init(this, options->GetDemoFilename()) ) {
		fprintf(stderr, "ERROR: InitSystem: failed to init input!\n");
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
int GameState::InitObjects() {
	
	// create some random objects
	Object* new_obj;
	
	int i, max = 5;
	objects.resize(0);

	new_obj = objectFactory->CreateObject(OBJECT_ID_BACKGROUND);
	if (!new_obj)
		return -1;
	
	objects.push_back(new_obj);
	
	for (i = 0; i < max; i++) {
		new_obj = objectFactory->CreateObject(OBJECT_ID_RADIUS_BLOCK);
		if (!new_obj)
			return -1;

		objects.push_back(new_obj);
	}
	
	new_obj = objectFactory->CreateObject(OBJECT_ID_MOUSE_BLOCK);
	if (!new_obj)
		return -1;

	objects.push_back(new_obj);
	
	return 0;
}

//! The 'main' function for the game

//! It takes a pointer to some game options (fullscreen/etc).
//! It initializes everything, and returns 0 if successful
//! or 1 on error.
int GameState::RunGame(GameOptions* _options) {
		
		options = _options;
		
		if (InitSystem() != -1 && InitObjects() != -1) {
			
			// XXX SHOULD NOT BE option-> should be input->RecordDemo()
			if (options->RecordDemo())
				input->BeginRecording();
			else if (options->PlaybackDemo())
				input->BeginPlayback();
			
			MainLoop();

			if (options->RecordDemo())
				input->EndRecording();
			else if (options->PlaybackDemo())
				input->EndPlayback();

		} else {
			fprintf(stderr, "ERROR: Failed to init game!\n");
			return -1;	
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

		while (outstanding_updates > 0) {
			Update();
			outstanding_updates--;
		}
		Draw();

		// wait for 1/60th sec to elapse (if we're a fast computa)
		while (outstanding_updates <= 0);
  }
}

//! Update all game status

//! Call update on all objects to let them know
//! how much time has passed.
void GameState::Update() {
	int i, max = objects.size();

	input->Update();

	for (i = 0; i < max; i++) {
		objects[i]->Update();
	}

	if (input->Key(GAMEKEY_EXIT)) {
		exit_game = true;
	}
}

//! Draw all game objects

//! Loop through and draw all game objects.
void GameState::Draw() {
	int i, max = objects.size();

	for (i = 0; i < max; i++) {
		objects[i]->Draw();
	}

	window->Flip();
}

//! Shutdown the game

//! Clean up everything we allocated
void GameState::Shutdown() {

	if (objectFactory) {
		DestroyObjects();
		objectFactory->Shutdown();
		delete objectFactory;
	}

	if (input) {
		input->Shutdown();
		delete input;
	}
		
	// window destruction code must be LAST
	if (window) {
		window->Shutdown();
		delete window;
	}
		
	allegro_exit();
}

//! Destory all game objects

//! Clean up everything we allocated
void GameState::DestroyObjects() {
	int i, max = objects.size();

	for (i = 0; i < max; i++) {
		objectFactory->DeleteObject(objects[i]);
	}
}

GameState::GameState() : 
window(NULL), objectFactory(NULL), input(NULL), objects(0) {}
GameState::~GameState() {}
