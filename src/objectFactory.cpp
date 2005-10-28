#include "objectFactory.h"

// XXX NASTY!! SO BADLY WRITTEN, CLEAN IT UP
// break this up into smaller functions.. COME ON MORON!!X0r.
// ALSO, tons of memory leaks.  We will break this thing down
//  For now it 'just works'
Object* ObjectFactory::CreateObject(uint id) {
	
	Object* new_obj = NULL;
	RadiusBlockObject *rblock;
	PlayerObject *player;
	BackgroundObject *bg;
	
	switch (id) {
		case OBJECT_ID_BACKGROUND:
			
			bg = new BackgroundObject();
			if ( bg && bg->Init(GetGameState()) ) {
							
				PALETTE pal;
				BITMAP* bmp = load_bitmap(get_correct_path("data/back.tga"), pal);
				
				if (bmp) {

					set_palette(pal);

					bg->SetBitmapIsDeleteable(true);
					bg->SetBitmap(bmp);
					bg->SetXY(0,0);
					
				} else {
					fprintf(stderr, "ObjectFactory: Failed to load Player's bitmap.\n");
					if (bg) {
							free(bg);
							bg = NULL;
					}
				}
			} else {
				fprintf(stderr, "ObjectFactory: Failed to create Player object.\n");
				if (bg) {
						free(bg);
						bg = NULL;
				}	
			}

			new_obj = bg;
			break;

					
		case OBJECT_ID_RADIUS_BLOCK:
			
			rblock = new RadiusBlockObject();
			if ( rblock && rblock->Init(GetGameState()) ) {
							
				PALETTE pal;
				BITMAP* bmp = load_bitmap(get_correct_path("data/miroku.tga"), pal);
				
				if (bmp) {

					set_palette(pal);

					rblock->SetBitmapIsDeleteable(true);
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
				BITMAP* bmp = load_bitmap(get_correct_path("data/kenshin.tga"), pal);
				
				if (bmp) {

					set_palette(pal);

					player->SetBitmapIsDeleteable(true);
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

int ObjectFactory::Init(GameState* _game_state) {
	SetGameState(_game_state);
	return 0;
}

void ObjectFactory::Shutdown() {
}

ObjectFactory::ObjectFactory() {}
ObjectFactory::~ObjectFactory() {} 
