/* Forces: Gravity
 * all time increments are 1/60th of a second */

#ifndef FORCEINPUT_H
#define FORCEINPUT_H

#include <allegro.h>

#include "globals.h"
#include "force.h"

class ForceInput : public Force {
	public:
		float GetAcceleration();

		ForceInput();
		virtual ~ForceInput();
};

#endif // FORCEINPUT_H
