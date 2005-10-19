#include "objectFactory.h"

void ObjectFactory::SetInputHandler(BaseInput* handler) {
	input = handler;
}

Object* ObjectFactory::CreateObject(uint id) {
	
	Object* new_obj = NULL;
	RadiusBlockObject *rblock;
	MouseBlockObject *mblock;
	
	switch (id) {
		case OBJECT_ID_RADIUS_BLOCK:
			
			rblock = new RadiusBlockObject();
			if (rblock) {
							
				PALETTE pal;
				BITMAP* bmp = load_bitmap("data/ninja.tga", pal);
				
				if (bmp) {

					set_palette(pal);

					rblock->SetBitmapIsDeleteable(true);
					rblock->SetDestinationBitmap(default_destination_bitmap);
					rblock->SetBitmap(bmp);
					rblock->SetXY(Rand(0, SCREEN_SIZE_X), Rand(0, SCREEN_SIZE_Y));
					rblock->SetTheta(Rand(0,360));
					rblock->SetRadius(Rand(20,300));
					
				} else {
					fprintf(stderr, "ObjectFactory: Failed to create Sonic sprite's bitmap.\n");
				}
			} else {
				fprintf(stderr, "ObjectFactory: Failed to create Sonic sprite.\n");
			}

			new_obj = rblock;
			break;
			
		case OBJECT_ID_MOUSE_BLOCK:
			
			mblock = new MouseBlockObject();
			if (mblock) {
							
				PALETTE pal;
				BITMAP* bmp = load_bitmap("data/ninja2.tga", pal);
				
				if (bmp) {

					set_palette(pal);

					mblock->SetBitmapIsDeleteable(true);
					mblock->SetDestinationBitmap(default_destination_bitmap);
					mblock->SetBitmap(bmp);
					
				} else {
					fprintf(stderr, "ObjectFactory: Failed to create Sonic sprite's bitmap.\n");
				}
		
			} else {
				fprintf(stderr, "ObjectFactory: Failed to create Sonic sprite.\n");
			}

			new_obj = mblock;
			break;

		default:
			fprintf(stderr, "ObjectFactory: Unknown ID passed: %i\n", id);
			new_obj = NULL;
	}
	
	return new_obj;
}

void ObjectFactory::DeleteObject(Object* obj) {
	obj->Delete();
	delete obj;
}

int ObjectFactory::Init(BaseInput* handler) {
	SetInputHandler(handler);
	return 0;
}

void ObjectFactory::SetDefaultDestinationBitmap(BITMAP* bmp) {
	default_destination_bitmap = bmp;
}

void ObjectFactory::Shutdown() {
}

ObjectFactory::ObjectFactory() : 
default_destination_bitmap(NULL), input(NULL) {
}

ObjectFactory::~ObjectFactory() {
}
