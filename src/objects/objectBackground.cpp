#include "objectBackground.h"

#include "globals.h"
#include "gameState.h"
#include "animation.h"
#include "physSimulation.h"
#include "object.h"
#include "window.h"

void BackgroundObject::Update() {
	BaseUpdate();
	UpdateSimpleAnimations();
}

// We want to wrap the background around the level.  Compute the offset
void BackgroundObject::Draw() {
	int x,y, i = 0;
				
	// XXX THIS CAN DEFINITELY BE SIMPLIFIED
	// Draw it a few times in case so we can "wrap" around
	// the screen
	do {
		Transform(x,y);
		x = - ( GetWidth() - ( x % GetWidth()) ) + i;
		// fprintf(stderr, "x,y=(%i,%i) ", x ,y);
	
		GetGameState()->GetWindow()->
    DrawSprite(currentSprite, x, y, flip_x, flip_y);

		i += GetWidth();
		
	} while ( x < (int)game_state->ScreenWidth() );
}

bool BackgroundObject::Init(GameState *_game_state, PhysSimulation *p) {
		SetGameState(_game_state);
		simulation = p;
		return BaseInit();
}

BackgroundObject::BackgroundObject() {}
BackgroundObject::~BackgroundObject() {}
