// this file is a TEST ONLY

#ifndef RADIUSBLOCKOBJECT_H
#define RADIUSBLOCKOBJECT_H

#define RADIUS_BLOCK_SPEED 0.01f

#include <math.h>
#include <stdio.h>

#include "object.h"

class RadiusBlockObject : public Object {
	protected:
		float theta;
		int radius;
				
	public:
		void Update();
		void Draw();

		void SetTheta(float angle);
		void SetRadius(int r);

		RadiusBlockObject();
		~RadiusBlockObject();
};

#endif // RadiusBlockObject_H   
