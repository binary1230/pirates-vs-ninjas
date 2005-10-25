#include "input.h"

// default game keys, failsafes only
#define DEFAULT_GAMEKEY_JUMP	KEY_SPACE
#define DEFAULT_GAMEKEY_EXIT	KEY_ESC
#define DEFAULT_GAMEKEY_LEFT	KEY_LEFT
#define DEFAULT_GAMEKEY_RIGHT 	KEY_RIGHT
#define DEFAULT_GAMEKEY_UP 	KEY_UP
#define DEFAULT_GAMEKEY_DOWN	KEY_DOWN

// pass key_buffer = NULL in order to use the default key buffer
// (which is usually what you want)
void BaseInput::ClearKeys(int* key_buffer) {

	if (!key_buffer)
		key_buffer = game_key;
		
	uint i;
	for (i = 0; i < GAMEKEY_COUNT; i++) {
		key_buffer[i] = 0;
	}
}

void BaseInput::LoadDefaultKeyMappings() {
	fprintf(stderr, "BaseInput: Using default key mappings!\n");
	
	gamekey_to_realkey[GAMEKEY_JUMP] = DEFAULT_GAMEKEY_JUMP;
	gamekey_to_realkey[GAMEKEY_EXIT] = DEFAULT_GAMEKEY_EXIT;
	gamekey_to_realkey[GAMEKEY_LEFT] = DEFAULT_GAMEKEY_LEFT;
	gamekey_to_realkey[GAMEKEY_RIGHT]= DEFAULT_GAMEKEY_RIGHT;
	gamekey_to_realkey[GAMEKEY_UP] 	 = DEFAULT_GAMEKEY_UP;
	gamekey_to_realkey[GAMEKEY_DOWN] = DEFAULT_GAMEKEY_DOWN;

	ClearKeys();
}

bool BaseInput::LoadKeyMappings(char* filename) {
	fprintf(stderr, "BaseInput: Key map loading not supported yet!\n");
	return false;
}

BaseInput::BaseInput() {}
BaseInput::~BaseInput() {}
