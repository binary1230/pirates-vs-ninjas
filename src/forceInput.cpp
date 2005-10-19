#include "forceInput.h"

float ForceInput::GetAcceleration() {
	// return a force based on 2 inputs.
	if (key[KEY_LEFT]) {
		return -0.3f;
	} else if (key[KEY_RIGHT]) {
		return 0.3f;
	} else {
		return 0.0f;	
	}
}

ForceInput::ForceInput() {
}

ForceInput::~ForceInput() {}
