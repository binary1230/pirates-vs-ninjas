#include "forceGravity.h"

float ForceGravity::GetAcceleration() {
				
		// quick physics info
		//  a = g = -9.81 m/sec^2 (gravity)
		//  t = 0.016666667 (1/60th of a second)
		//  at = -0.1635

		return -0.1635;
}

ForceGravity::ForceGravity() {}
ForceGravity::~ForceGravity() {}
