// this file is a TEST ONLY

#ifndef MOUSEBLOCKOBJECT_H
#define MOUSEBLOCKOBJECT_H

#include <allegro.h>
#include <math.h>
#include <stdio.h>
#include <vector>

using namespace std;

#include "globals.h"
#include "input.h"
#include "inputLiveHandler.h"
#include "object.h"
#include "vec.h"

#include "forceGravity.h"
#include "forceInput.h"

#define _X 0
#define _Y 1

class MouseBlockObject : public Object {
	protected:
		BaseInput* input;
		vec vectors[2];		// vectors for X and Y
				
	public:
		void Update();
		void Draw();
		void SetInputHandler(BaseInput* handler);

		void SetXY(int, int);

		MouseBlockObject();
		~MouseBlockObject();
};

#endif // MouseBlockObject_H   
