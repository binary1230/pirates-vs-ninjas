#include "objectFactory.h"

void ObjectFactory::SetInputHandler(BaseInput* handler) {
	input = handler;
}

// XXX NASTY!! SO BADLY WRITTEN, CLEAN IT UP
Object* ObjectFactory::CreateObject(uint id) {
	
	Object* new_obj = NULL;
	RadiusBlockObject *rblock;
	PlayerObject *player;
	
	switch (id) {
		case OBJECT_ID_RADIUS_BLOCK:
			
			rblock = new RadiusBlockObject();
			if ( rblock && rblock->Init(GetGameState()) ) {
							
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
					fprintf(stderr, "ObjectFactory: Failed to load Player's bitmap.\n");
					if (rblock) {
							free(rblock);
							rblock = NULL;
					}
				}
			} else {
				fprintf(stderr, "ObjectFactory: Failed to create Player object.\n");
				if (rblock) {
						free(rblock);
						rblock = NULL;
				}	
			}

			new_obj = rblock;
			break;
			
		case OBJECT_ID_MOUSE_BLOCK:
			
			player = new PlayerObject();

			if ( player && player->Init(GetGameState()) ) {
							
				PALETTE pal;
				BITMAP* bmp = load_bitmap("data/ninja2.tga", pal);
				
				if (bmp) {

					set_palette(pal);

					player->SetInputHandler(input);
					player->SetBitmapIsDeleteable(true);
					player->SetDestinationBitmap(default_destination_bitmap);
					player->SetBitmap(bmp);
					
				} else {
					fprintf(stderr, "ObjectFactory: Failed to create Sonic sprite's bitmap.\n");
					if (player) {
							free(player);
							player = NULL;
					}	
				}
		
			} else {
				fprintf(stderr, "ObjectFactory: Failed to create Sonic sprite.\n");
				if (player) {
						free(player);
						player = NULL;
				}
			}

			new_obj = player;
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

int ObjectFactory::Init(GameState* _game_state, BaseInput* handler) {
	SetGameState(_game_state);
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
