#include "forceInput.h"

float ForceInput::GetAcceleration() {

	if (!GetGameState()) {
			fprintf(stderr, "Gamestate is NULL??\n");
			abort();
	}

	// return a force based on 2 inputs.
	if (GetGameState()->GetInput()->Key(GAMEKEY_LEFT)) {
		return -0.3f;
	} else if (GetGameState()->GetInput()->Key(GAMEKEY_RIGHT)) {
		return 0.3f;
	} else {
		return 0.0f;	
	}
}

ForceInput::ForceInput() {
}

ForceInput::~ForceInput() {}
