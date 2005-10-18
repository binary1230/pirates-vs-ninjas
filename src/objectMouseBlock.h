// this file is a TEST ONLY

#ifndef MOUSEBLOCKOBJECT_H
#define MOUSEBLOCKOBJECT_H

#include <math.h>
#include <stdio.h>

#include "object.h"

class MouseBlockObject : public Object {
	protected:
				
	public:
		void Update();
		void Draw();

		MouseBlockObject();
		~MouseBlockObject();
};

#endif // MouseBlockObject_H   
