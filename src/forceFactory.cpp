#include "forceFactory.h"
#include "globals.h"
#include "forceInput.h"
#include "forceGravity.h"
#include "force.h"

// XXX NASTY!! SO BADLY WRITTEN, CLEAN IT UP
Force* ForceFactory::CreateForce(ForceType type) {
	
	Force* force = NULL;
	ForceInput* inputForce;
	ForceGravity* gravityForce;
	int controller_num = 0;
	
	switch (type) {
		case FORCE_INPUT1:
		case FORCE_INPUT2:
						
			if (type == FORCE_INPUT1) {
				controller_num = 1;
			} else if (type == FORCE_INPUT2) {
				controller_num = 2;
			}
							
			inputForce = new ForceInput();
			if ( inputForce && inputForce->Init() ) {
				
				inputForce->SetControllerNum(controller_num);
			
			} else {
				fprintf(stderr, "ForceFactory: Failed to create new Input Force\n");
				if (inputForce) {
						free(inputForce);
						inputForce = NULL;
				}	
			}

			force = inputForce;
			break;

		case FORCE_GRAVITY:
			gravityForce = new ForceGravity();
			if ( gravityForce && gravityForce->Init() ) {

				// Initialize it here
				// gravityForce->SetSomething(...)
			
			} else {
				fprintf(stderr, "ForceFactory: Failed to create new Input Force\n");
				if (gravityForce) {
						free(gravityForce);
						gravityForce = NULL;
				}	
			}

			force = gravityForce;
			break;
		default:
			fprintf(stderr, "ForceFactory: Unknown ID passed: %i\n", type);
			force = NULL;
	}
	
	return force;
}

/*void ForceFactory::DeleteForce(Force* obj) {
	obj->Delete();
	delete obj;
}*/

int ForceFactory::Init() {
	return 0;
}

void ForceFactory::Shutdown() {
}

ForceFactory::ForceFactory() {}
ForceFactory::~ForceFactory() {} 
