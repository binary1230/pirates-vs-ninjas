#include "objectRadiusBlock.h"

#include "globals.h"
#include "animation.h"
#include "gameState.h"
#include "physSimulation.h"

void RadiusBlockObject::Update() {
	UpdateFade();

	if (currentAnimation) {
		currentAnimation->Update();
		currentSprite = currentAnimation->GetCurrentSprite();
	}
	theta += RADIUS_BLOCK_SPEED;
	pos.SetX( int(sinf(theta) * radius) ); 
	pos.SetY( int(cosf(theta) * radius) );
}

bool RadiusBlockObject::Init(GameState *_game_state) {
	SetGameState(_game_state);
	return BaseInit();
}

RadiusBlockObject::RadiusBlockObject() : theta(0.0f) {}
RadiusBlockObject::~RadiusBlockObject() {}
