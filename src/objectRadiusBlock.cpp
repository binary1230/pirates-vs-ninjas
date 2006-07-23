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

// Factory method, creates new RadiusBlockObjects
// XXX animation stuff needs to be abstracted
Object* RadiusBlockObject::New(	GameState* gameState, 
																XMLNode &xDef, 
																XMLNode &xObj) {
	RadiusBlockObject* obj = new RadiusBlockObject();

	if (!obj || !obj->Init(gameState) )
		return NULL;

	obj->SetTheta(Rand(0,360));
	obj->SetRadius(Rand(20,300));

	if (!obj->LoadAnimations(xDef))
		return NULL;
	
	if (!obj->LoadProperties(xDef))
		return NULL;

	return obj;
}
