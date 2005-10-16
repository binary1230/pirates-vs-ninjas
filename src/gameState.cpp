#include "gameState.h"

int GameState::Init() {

		exit_game = false;

		if ( (window = new Window()) ) 
			return window->Init(320,240,0);
		else 
			return -1;
		
}

void GameState::Update() {}

void GameState::Draw() {}

// the 'main' function for the game
// XXX todo: pass options like cheats/etc to this function
// don't pass argc, argv, parse them in main() and give
// this an array to chew on.
int GameState::RunGame() {
		
		int return_val = 0;
		
		return_val = Init();
		
		while (!exit_game) {
				Draw();
				Update();
		}

		Shutdown();

		return return_val;
}

void GameState::Shutdown() {
	if (window) {
		window->Shutdown();
		delete window;
	}
}

GameState::GameState() : window(NULL) {}
GameState::~GameState() {}
