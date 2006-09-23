#include "forceInput.h"
#include "globals.h"
#include "input.h"
#include "gameState.h"
#include "object.h"

#define MAGNITUDE 40.0f

Vector2D ForceInput::GetForce(Object* obj) {
				
	if (obj->GetControllerNum() != controller_num)
		return Vector2D(0,0);

	// return a force based on 2 inputs.
	if (GAMESTATE->GetKey(PLAYERKEY_LEFT, controller_num) && 
			!GAMESTATE->GetKey(PLAYERKEY_RIGHT, controller_num)) 
		return Vector2D(-MAGNITUDE * TIMESTEP, 0.0f);

	else if (GAMESTATE->GetKey(PLAYERKEY_RIGHT, controller_num) && 
					!GAMESTATE->GetKey(PLAYERKEY_LEFT, controller_num)) 
		return Vector2D( MAGNITUDE * TIMESTEP, 0.0f);

	else
		return Vector2D( 0.0f, 0.0f);	
}

ForceInput::ForceInput() {
	type = FORCE_INPUT;
	controller_num = 1;	// default
}

ForceInput::~ForceInput() {}
