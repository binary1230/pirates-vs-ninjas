#include "objectTxtOverlay.h"

#include "assetManager.h"
#include "window.h"
#include "sprite.h"
#include "input.h"
#include "gameState.h"
#include "physSimulation.h"

#define DEFAULT_BOX_COLOR	 	makecol(172, 20, 31)
#define DEFAULT_BOX_MARGIN	10
#define DEFAULT_BOX_HEIGHT	100

int ObjectText::GetHeight() {
	return box_height;
}

int ObjectText::GetWidth() {
	return WINDOW->Width() - (box_margin * 2);
}

void ObjectText::Draw() {

	const int box_x1 = box_margin;
	const int box_y1 = WINDOW->Height() - (box_margin + box_height);
	const int box_x2 = WINDOW->Width() - (box_margin * 2);
	const int box_y2 = WINDOW->Height() - box_margin;

	/* draw underlying box */
	WINDOW->DrawFillRect(	box_x1,	box_y1, 
												box_x2,	box_y2,
												box_color);

	/* draw avatar */
	WINDOW->DrawSprite(avatar_sprite, box_x1, box_y1);

	/* draw text */
	WINDOW->DrawText(	box_x1 + box_margin + avatar_sprite->bmp->w, 
										box_y1 + box_margin, text);
}

void ObjectText::Update() {
	BaseUpdate();	
	
	// if we're modal, NOTHING else can happen
	// until the user presses a key
	if (is_modal) {
		if (INPUT->KeyOnce(PLAYERKEY_JUMP, 1)) {
			SetModalActive(false);
			is_dead = true;
		}
	}
}

void ObjectText::SetModalActive(bool state) {
	is_modal = state;

	if (is_modal)
		simulation->SetModalObject(this);
	else
		simulation->SetModalObject(NULL);
}

bool ObjectText::Init(PhysSimulation *p) {
	simulation = p;
	is_modal = false;
	avatar_sprite = NULL;
	box_margin 	= DEFAULT_BOX_MARGIN;
	box_color		=	DEFAULT_BOX_COLOR;
	box_height	= DEFAULT_BOX_HEIGHT;
	text = "TEST: All your base are belong to us.";

	return BaseInit();
}

bool ObjectText::SetAvatarFilename(CString file) {
	if (!avatar_sprite)
		avatar_sprite = new Sprite();

	avatar_sprite->bmp = ASSETMANAGER->LoadBitmap(file.c_str());
	avatar_sprite->x_offset = 5;
	avatar_sprite->y_offset = 5;

	return (avatar_sprite->bmp != NULL);
}

void ObjectText::Shutdown() {
	if (avatar_sprite)
		delete avatar_sprite;
}

ObjectText::ObjectText() {
	is_modal = false;
	avatar_sprite = NULL;
}

ObjectText::~ObjectText() {
}
