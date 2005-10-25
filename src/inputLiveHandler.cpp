#include "inputLiveHandler.h"

int InputLive::Init(GameState* _game_state) {
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

bool InputLive::Key(uint gameKey) {
	return game_key[gameKey];
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
