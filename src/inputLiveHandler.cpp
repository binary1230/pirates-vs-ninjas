#include "inputLiveHandler.h"

// default game keys, failsafes only
#define DEFAULT_GAMEKEY_JUMP	KEY_SPACE
#define DEFAULT_GAMEKEY_EXIT	KEY_ESC
#define DEFAULT_GAMEKEY_LEFT	KEY_LEFT
#define DEFAULT_GAMEKEY_RIGHT 	KEY_RIGHT
#define DEFAULT_GAMEKEY_UP 	KEY_UP
#define DEFAULT_GAMEKEY_DOWN	KEY_DOWN

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

void InputLive::ClearKeys() {
	int i;
	for (i = 0; i < GAMEKEY_COUNT; i++) {
		game_key[i] = 0;
	}
}

void InputLive::LoadDefaultKeyMappings() {
	fprintf(stderr, "InputLive: Using default key mappings!\n");
	
	gamekey_to_realkey[GAMEKEY_JUMP] = DEFAULT_GAMEKEY_JUMP;
	gamekey_to_realkey[GAMEKEY_EXIT] = DEFAULT_GAMEKEY_EXIT;
	gamekey_to_realkey[GAMEKEY_LEFT] = DEFAULT_GAMEKEY_LEFT;
	gamekey_to_realkey[GAMEKEY_RIGHT]= DEFAULT_GAMEKEY_RIGHT;
	gamekey_to_realkey[GAMEKEY_UP] 	 = DEFAULT_GAMEKEY_UP;
	gamekey_to_realkey[GAMEKEY_DOWN] = DEFAULT_GAMEKEY_DOWN;

	ClearKeys();
}

bool InputLive::LoadKeyMappings(char* filename) {
	fprintf(stderr, "InputLive: Key map loading not supported yet!\n");
	return false;
}

bool InputLive::Key(enum GameKeys gameKey) {
	return game_key[gameKey];
}

void InputLive::Update() {
	// freeze the current state of the input into gamekey[].
	// key[] is from allegro: the current state of what is
	// currently being pressed
	int i;
	for (i = 0; i < GAMEKEY_COUNT; i++) {
		game_key[i] = key[gamekey_to_realkey[i]];
	}
}

InputLive::InputLive() {}
InputLive::~InputLive() {}
