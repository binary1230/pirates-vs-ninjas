#include "objectRadiusBlock.h"

void RadiusBlockObject::Update() {
	theta += RADIUS_BLOCK_SPEED;
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
