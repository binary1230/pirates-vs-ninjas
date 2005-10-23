/* A force vector
 * all time increments are 1/60th of a second
 */

#ifndef FORCE_H
#define FORCE_H

#include "globals.h"

class Force {
	public:
		virtual float GetAcceleration() = 0;

		Force();
		virtual ~Force();
};

#endif // FORCE_H
