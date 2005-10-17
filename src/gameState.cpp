#include "gameState.h"

int GameState::InitSystem() {

		exit_game = false;

		window = new Window();
		if ( !window || window->Init(320,240,0) < 0 )
			return -1;

		objectFactory = new ObjectFactory();
		if ( !objectFactory || objectFactory->Init() < 0 )
			return -1;

		objectFactory->SetDefaultDestinationBitmap(window->GetBackBuffer());

		return 0;
}

int GameState::InitObjects() {
	
	// create some random objects
	objects.resize(200);
	int i, max = objects.size();

	for (i = 0; i < max; i++) {
		objects[i] = objectFactory->CreateObject(OBJECT_ID_SONIC);
	}
				
	return 0;
}

void GameState::DestroyObjects() {
	int i, max = objects.size();

	for (i = 0; i < max; i++) {
		objectFactory->DeleteObject(objects[i]);
	}
}

void GameState::Update() {
	int i, max = objects.size();

	for (i = 0; i < max; i++) {
		objects[i]->Update();
	}
}

void GameState::Draw() {
	int i, max = objects.size();

	for (i = 0; i < max; i++) {
		objects[i]->Draw();
	}

	window->Flip();
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
		Draw();
		Update();
	}
}

void GameState::Shutdown() {
	if (window) {
		window->Shutdown();
		delete window;
	}

	if (objectFactory) {
		DestroyObjects();
		objectFactory->Shutdown();
		delete objectFactory;
	}
}

GameState::GameState() : window(NULL), objects(0) {}
GameState::~GameState() {}
