#ifndef GAMEVEC_H
#define GAMEVEC_H

#include <stdio.h>
#include <math.h>
#include <vector>

using namespace std;

#include "force.h"

class vec {
	public:
		float position;
		float velocity;
		float acceleration;

		float v_decay;
		
		vector<Force*> forces;

		int CalcNextStep();		// calc the next step (1/60th sec later)
													// update and return the next position
		
		int GetPosition();		// just return the current position

		void Clear();					// set all values to 0
		void Attach(Force*);	// attach a new force

		vec();
		~vec();
};

#endif // GAMEVEC_H
