/* Forces: Gravity
 * all time increments are 1/60th of a second */

#ifndef FORCEGRAV_H
#define FORCEGRAV_H

class ForceGravity;
class Force;

#include "globals.h"
#include "force.h"

class ForceGravity : public Force {
	public:
		float GetAcceleration();

		ForceGravity();
		virtual ~ForceGravity();
};

#endif // FORCEGRAB_H
