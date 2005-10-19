#include "gameState.h"

int GameState::InitSystem() {

		exit_game = false;

		allegro_init();
		
		InitTimers();

		window = new Window();
		if ( !window || window->Init(SCREEN_SIZE_X, SCREEN_SIZE_Y, 0) < 0 ) {
			fprintf(stderr, "InitSystem: failed to init window!\n");
			return -1;
		}

		input = new InputLive();
		if ( !input || input->Init() < 0 ) {
			fprintf(stderr, "InitSystem: failed to init input!\n");
			return -1;
		}

		objectFactory = new ObjectFactory();
		if ( !objectFactory || objectFactory->Init(input) < 0 ) {
			fprintf(stderr, "InitSystem: failed to init objectFactory!\n");
			return -1;
		}
					
		objectFactory->SetDefaultDestinationBitmap(window->GetBackBuffer());

		return 0;
}

int GameState::InitTimers() {
	install_timer();
	LOCK_VARIABLE(outstanding_updates);
	LOCK_FUNCTION((void*)Timer);
	return install_int_ex(Timer, BPS_TO_TIMER(60));
}

int GameState::InitObjects() {
	
	// create some random objects
	Object* new_obj;
	
	int i, max = 30;
	objects.resize(0);

	for (i = 0; i < max; i++) {
		new_obj = objectFactory->CreateObject(OBJECT_ID_RADIUS_BLOCK);
		if (!new_obj)
			return -1;

		objects.push_back(new_obj);
	}
		
	new_obj = objectFactory->CreateObject(OBJECT_ID_MOUSE_BLOCK);
	objects.push_back(new_obj);
	
	return 0;
}

// the 'main' function for the game
// XXX todo: pass options like cheats/etc to this function
// don't pass argc, argv, parse them in main() and give
// this an array to chew on.
int GameState::RunGame() {
		
		int return_val = 0;
		
		return_val = InitSystem();

		if (return_val != -1) {
			return_val = InitObjects();

			if (return_val != -1) 
				MainLoop();
		}
	
		Shutdown();

		return return_val;
}

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

void GameState::Update() {
	int i, max = objects.size();

	for (i = 0; i < max; i++) {
		objects[i]->Update();
	}

	if (input->Key(KEY_ESC)) {
		exit_game = true;
	}
}

void GameState::Draw() {
	int i, max = objects.size();

	for (i = 0; i < max; i++) {
		objects[i]->Draw();
	}

	window->Flip();
}

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

void GameState::DestroyObjects() {
	int i, max = objects.size();

	for (i = 0; i < max; i++) {
		objectFactory->DeleteObject(objects[i]);
	}
}

GameState::GameState() : window(NULL), objects(0) {}
GameState::~GameState() {}
