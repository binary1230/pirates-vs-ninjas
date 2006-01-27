#ifndef FORCEINPUT_H
#define FORCEINPUT_H

#include <allegro.h>

class ForceInput;
class Object;

#include "force.h"
#include "globals.h"

//! A Force generated in response to keystrokes (e.g. LEFT or RIGHT)
class ForceInput : public Force {
	public:
		Vector2D GetForce(Object*);

		//! Which controller we look at
		uint controller_num;

		//! Set which controller we look at
		void SetControllerNum(uint _c) {controller_num = _c;};
		
		ForceInput();
		virtual ~ForceInput();
};

#endif // FORCEINPUT_H
