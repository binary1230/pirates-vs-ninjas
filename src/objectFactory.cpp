#include "objectFactory.h"

Object* ObjectFactory::CreateObject(uint id) {
	
	Object* new_obj = NULL;
	
	switch (id) {
		case OBJECT_ID_SONIC:
			
			RadiusBlockObject *block = new RadiusBlockObject();
			if (block) {
							
				PALETTE pal;
				BITMAP* bmp = load_bitmap("ninja.tga", pal);
				
				//  = create_bitmap(32, 32);
		
				if (bmp) {

					set_palette(pal);
					// clear_to_color(bmp, Rand(8,10) );

					block->SetBitmapIsDeleteable(true);
					block->SetDestinationBitmap(default_destination_bitmap);
					block->SetBitmap(bmp);
					block->SetXY(Rand(0,320), Rand(0,240));
					block->SetTheta(Rand(0,360));
					block->SetRadius(Rand(20,300));
					
				} else {
					fprintf(stderr, "ObjectFactory: Failed to create Sonic sprite's bitmap.\n");
				}
		
			} else {
				fprintf(stderr, "ObjectFactory: Failed to create Sonic sprite.\n");
			}

			new_obj = block;
			
			break;

		default:
			fprintf(stderr, "ObjectFactory: Unknown ID passed: %i\n", id);
	}
	
	return new_obj;
}

void ObjectFactory::DeleteObject(Object* obj) {
	obj->Delete();
	delete obj;
}

int ObjectFactory::Init() {
	return 0;
}

void ObjectFactory::SetDefaultDestinationBitmap(BITMAP* bmp) {
	default_destination_bitmap = bmp;
}

void ObjectFactory::Shutdown() {
}

ObjectFactory::ObjectFactory() : default_destination_bitmap(NULL) {
}

ObjectFactory::~ObjectFactory() {
}
