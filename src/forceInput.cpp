#include "forceInput.h"

float ForceInput::GetAcceleration() {
	// return a force based on 2 inputs.
	if (key[KEY_LEFT]) {
		acc = -0.2f;
	} else if (key[KEY_RIGHT]) {
		acc = 0.2f;
	} else /*if (acc >= 0.01)*/ {
		//acc -= acc*decay;
		//if (acc < 0.01) {
			acc = 0;	
		//}
	}

	return acc;
}

ForceInput::ForceInput() {
	acc = 0;
	decay = 0.01;
}
ForceInput::~ForceInput() {}
