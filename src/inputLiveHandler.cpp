#include "inputLiveHandler.h"

int InputLive::Init(GameState* _game_state, CString _demo_file) {
				
	SetGameState(_game_state);
				
	install_mouse();
	install_keyboard();

	if (!BaseInit()) {
		return -1;
	}
	
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

	UpdateKeyReleases();
}

InputLive::InputLive() {}
InputLive::~InputLive() {}
