#include "objectBackground.h"
#include "globals.h"
#include "gameState.h"
#include "animation.h"
#include "physSimulation.h"
#include "object.h"
#include "window.h"

void BackgroundObject::Update() {
	// pos.SetX(0);

	if (currentAnimation) {
		currentAnimation->Update();
		currentSprite = currentAnimation->GetCurrentSprite();
	}
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

bool BackgroundObject::Init(GameState *_game_state) {
		SetGameState(_game_state);
		return BaseInit();
}

Object* BackgroundObject::New(	GameState* gameState, 
																XMLNode &xDef, 
																XMLNode &xObj) {
	
	BackgroundObject* obj = new BackgroundObject();
	
	if (!obj || !obj->Init(gameState) )
		return NULL;

	obj->SetXY(0,0);

	if (!obj->LoadAnimations(xDef) )
		return NULL;
	
	if (!obj->LoadProperties(xDef))
		return NULL;
	
	return obj;
}

BackgroundObject::BackgroundObject() {}
BackgroundObject::~BackgroundObject() {}
