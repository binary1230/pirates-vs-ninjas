#include "objectFactory.h"

// XXX: just a placeholder so we can display _something_
Object* ObjectFactory::CreateObject(uint id) {
	
	Object* new_object = NULL;
	switch (id) {
		case OBJECT_ID_SONIC:
			// create the sonic sprite
			
			/* something like:
			new_object = new SonicObject();
			SonicObject->SetBitmap(SONIC);
			SonicObject->SetControls(Player1);
			*/
			break;

		default:
			fprintf(stderr, "ObjectFactory: Unknown ID passed: %i\n", id);
	}
	
	return new_object;
}

void ObjectFactory::DeleteObject(Object* obj) {
	delete obj;
}

ObjectFactory::ObjectFactory() {
}

ObjectFactory::~ObjectFactory() {
}
