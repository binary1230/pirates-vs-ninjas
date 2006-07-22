#include "objectController.h"
#include "resourceLoader.h"
#include "window.h"
#include "sprite.h"
#include "xmlParser.h"
#include "input.h"
#include "gameState.h"
#include "physSimulation.h"
#include "StdString.h"

int ControllerObject::GetHeight() {
	return controller_sprite->bmp->h;
}

int ControllerObject::GetWidth() {
	return controller_sprite->bmp->w;
}

void ControllerObject::Draw() {

	if (only_show_during_demo && !GetGameState()->IsPlayingBackDemo()) {
		return;
	}
				
	int x = (int)pos.GetX();
	int y = (int)pos.GetY();

	int bx, by;

	// Draw the base controller
	GetGameState()->GetWindow()->DrawSprite(controller_sprite, x, y);

	// Draw each button if it is active
	int i, max = buttons.size();
	for (i = 0; i < max; i++) {
		if (buttons[i].active) {
			bx = buttons[i].sprite->x_offset + x;
			by = buttons[i].sprite->y_offset + y;
			GetGameState()->GetWindow()->DrawBitmap(buttons[i].sprite->bmp, bx, by);
		}
	}
}

void ControllerObject::Update() {

	// keys, in the order shown on the controller
	int keys[] = {
		PLAYERKEY_LEFT,
		PLAYERKEY_RIGHT,
		PLAYERKEY_UP,
		PLAYERKEY_DOWN,
		PLAYERKEY_JUMP,
		-1
	};				

	int i, max = buttons.size();

	for (i = 0; i < max && keys[i] != -1; i++) {
		if (GetGameState()->GetKey(keys[i], controller_num))
			buttons[i].active = 1;
		else 
			buttons[i].active = 0;
	}
}

bool ControllerObject::Init(GameState* _game_state) {
	SetGameState(_game_state);
	
	buttons.clear();
	controller_sprite = NULL;
	controller_num = 1; // we'll set this later

	only_show_during_demo = false;
	
	return BaseInit();
}

void ControllerObject::Shutdown() {
	int i, max = buttons.size();
	
	for (i = 0; i < max; i++) {
		if (buttons[i].sprite) {
			buttons[i].sprite->Shutdown();
			delete buttons[i].sprite;
			buttons[i].sprite = NULL;
		}
	}

	if (controller_sprite) {
		controller_sprite->Shutdown();
		delete controller_sprite;
		controller_sprite = NULL;
	}
}

Object* ControllerObject::New(	GameState* _game_state, 
																XMLNode &xDef,
																XMLNode &xObj) {
	ControllerObject* obj = new ControllerObject();

	if (!obj || !obj->Init(_game_state) )
		return NULL;

	ObjectProperties props;
	props.is_overlay = 1;
	obj->SetProperties(props);

	// XXX READ which controller we monitor from XML file
	// but not in this method

	int i, iterator, max;
	CString filename;
	XMLNode xImages, xBtn;
	
	xImages = xDef.getChildNode("images");
	max = xImages.nChildNode("btn");

	obj->buttons.clear();
	obj->buttons.resize(max);

	filename = xImages.getChildNode("base").getText();
	obj->controller_sprite = new Sprite();
	obj->controller_sprite->bitmap_is_deleteable = true;
	
	CString file = obj->GetGameState()->GetResourceLoader()->GetPathOf(filename);
	obj->controller_sprite->bmp = load_bitmap(file, NULL);
		
	if (!obj->controller_sprite->bmp) {
	fprintf(stderr, "-- ERROR: Can't load file '%s'\n", filename.c_str() );
		delete obj->controller_sprite;
		delete obj;
		return NULL;
	}
	
	obj->controller_sprite->x_offset = 0;
	obj->controller_sprite->y_offset = 0;
	obj->pos.SetX(xImages.getChildNode("base").getChildNode("x").getInt());
	obj->pos.SetY(xImages.getChildNode("base").getChildNode("y").getInt());

	Button* b;
	
	// load each button
	max = obj->buttons.size();
	for (i=iterator=0; i < max; i++) {
			xBtn = xImages.getChildNode("btn", &iterator);
			b = &obj->buttons[i];
			
			filename = xBtn.getText();
			b->sprite = new Sprite();
			b->sprite->bitmap_is_deleteable = true;
			b->active = 0;
		
			file = obj->GetGameState()->GetResourceLoader()->GetPathOf(filename);
			b->sprite->bmp = load_bitmap(file, NULL);
			
			if (!b->sprite->bmp) {
				fprintf(stderr, "-- ERROR: Can't load file '%s'\n", filename.c_str() );
				return NULL;
			}

			b->sprite->x_offset = xBtn.getChildNode("x").getInt();
			b->sprite->y_offset = xBtn.getChildNode("y").getInt();
	}

	if (xDef.nChildNode("showDuringDemoOnly") > 0)
		obj->only_show_during_demo = true;
	
	if (!obj->LoadProperties(xDef))
		return NULL;
	
	return obj;
}

ControllerObject::ControllerObject() {
	controller_sprite = NULL;
}

ControllerObject::~ControllerObject() {}
