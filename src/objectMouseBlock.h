// this file is a TEST ONLY

#ifndef MOUSEBLOCKOBJECT_H
#define MOUSEBLOCKOBJECT_H

#include <allegro.h>
#include <math.h>
#include <stdio.h>

#include "input.h"
#include "inputLiveHandler.h"
#include "object.h"

class MouseBlockObject : public Object {
	protected:
		BaseInput* input;
				
	public:
		void Update();
		void Draw();
		void SetInputHandler(BaseInput* handler);

		MouseBlockObject();
		~MouseBlockObject();
};

#endif // MouseBlockObject_H   
