/* Forces: Gravity
 * all time increments are 1/60th of a second */

#ifndef FORCEINPUT_H
#define FORCEINPUT_H

#include <allegro.h>

class Force;
class ForceInput;

#include "globals.h"
#include "force.h"
#include "input.h"
#include "gameState.h"

class ForceInput : public Force {
	public:
		float GetAcceleration();

		ForceInput();
		virtual ~ForceInput();
};

#endif // FORCEINPUT_H
