#include "inputLiveHandler.h"

int InputLive::Init(GameState* _game_state, char* _demo_file) {

	if (_demo_file) {
		fprintf(stderr, "ERROR: Can't use a demo file with Live Input.\n");
		return -1;
	}
				
	SetGameState(_game_state);
				
	install_mouse();
	install_keyboard();
	
	LoadDefaultKeyMappings();
	return 0;
}

void InputLive::Shutdown() {
	remove_mouse();
	remove_keyboard();
}

//! Input update (freeze state of input)

//! freeze the current state of the input into gamekey[].
//! key[] is from allegro, it is the current state of what 
//! is currently being pressed
void InputLive::Update() {
	uint i;
	for (i = 0; i < GAMEKEY_COUNT; i++) {
		game_key[i] = key[gamekey_to_realkey[i]];
	}
}

InputLive::InputLive() {}
InputLive::~InputLive() {}
