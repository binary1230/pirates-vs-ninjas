#include "vec.h"

void vec::Clear() {
	position = 0;
	velocity = 0;
	acceleration = 0;
	forces.resize(0);
}

void vec::Attach(Force* x) {
	forces.push_back(x);	
}

// calculate the sum of the forces
// acting on this vector at 1/60th 
// of a second later.  we store things
// internally as float, but return ints
int vec::CalcNextStep() {

	int i, max = forces.size();
	
	// 1) calc sum of forces to get new acceleration
	acceleration = 0;
	for (i = 0; i < max; i++)
		acceleration += forces[i]->GetAcceleration();		
				
	// 2) add acceleration to velocity to get new velocity
	velocity = (velocity + acceleration) * v_decay;
	if (velocity < 0.01 && velocity > -0.01)
			velocity = 0;
	
	// 3) add velocity to position to get new position
	position += velocity;
	
	// 4) return the new position
	return GetPosition();
}

// return a truncated version of the position
int vec::GetPosition() {
		return (int)nearbyintf(position);
}

vec::vec() : v_decay(1.00), forces(0) {}
vec::~vec() {}
