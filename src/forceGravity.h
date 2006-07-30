#ifndef FORCEGRAV_H
#define FORCEGRAV_H

class ForceGravity;
class Force;
class Object;

#include "force.h"

//! A Force based on gravity
class ForceGravity : public Force {
	protected:

		//! The gravitational constant, 'g' (usually -9.81m/s^2)
		float gravitational_constant;
		
	public:
		Vector2D GetForce(Object*);

		ForceGravity(float _magnitude = -50.0f);
		virtual ~ForceGravity();
};

#endif // FORCEGRAV_H
