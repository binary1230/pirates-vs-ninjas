/* Forces: Gravity
 * all time increments are 1/60th of a second */

#ifndef FORCEGRAV_H
#define FORCEGRAV_H

#include "force.h"

class ForceGravity : public Force {
	public:
		float GetAcceleration();

		ForceGravity();
		virtual ~ForceGravity();
};

#endif // FORCEGRAB_H
