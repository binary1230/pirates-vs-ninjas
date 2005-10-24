#include "objectRadiusBlock.h"

void RadiusBlockObject::Update() {
	theta += RADIUS_BLOCK_SPEED;
}

bool RadiusBlockObject::Init(GameState *_game_state) {
		SetGameState(_game_state);
}

void RadiusBlockObject::Shutdown() {
				
}

void RadiusBlockObject::Draw() {
	DrawAtOffset(	int(sinf(theta) * radius) , 
								int(cosf(theta) * radius) );
}

void RadiusBlockObject::SetTheta(float angle) {
	theta = angle;
}

void RadiusBlockObject::SetRadius(int r) {
	radius = r;
}

RadiusBlockObject::RadiusBlockObject() : theta(0.0f) {}
RadiusBlockObject::~RadiusBlockObject() {}
