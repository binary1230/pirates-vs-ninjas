#include "forceGravity.h"
#include "globals.h"
#include "object.h"

/* note: if you are trying to figure out the engine, this file is pretty easy
 * to understand */

Vector2D ForceGravity::GetForce(Object* obj) {
		
		// we need to return the vector representing the force of gravity 
		// acting on this object in a given amount of time (the TIMESTEP)
		
		// the formula:
		//  gravitational_constant = -9.81 m/sec^2 (on earth)
		//  force = mass_of_object * gravitational_constant 
		
		float force = gravitational_constant * obj->GetMass() * TIMESTEP;
		return Vector2D(0.0f, force); 
}

ForceGravity::ForceGravity(float _gravitational_constant) {
	gravitational_constant = _gravitational_constant;
	type = FORCE_GRAVITY;
}

ForceGravity::~ForceGravity() {}
