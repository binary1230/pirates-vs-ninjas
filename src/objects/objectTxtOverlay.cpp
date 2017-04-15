#include "stdafx.h"
#include "objectTxtOverlay.h"

#include "assetManager.h"
#include "window.h"
#include "sprite.h"
#include "input.h"
#include "gameState.h"
#include "gameWorld.h"
#include "eventManager.h"

#define DEFAULT_BOX_COLOR	 	al_map_rgb(172, 20, 31)
#define DEFAULT_BOX_MARGIN	10
#define DEFAULT_BOX_HEIGHT	110

#define BLINK_TIME 10

int ObjectText::GetHeight() {
	return box_height;
}

int ObjectText::GetWidth() {
	return WINDOW->Width() - (box_margin * 2);
}

void ObjectText::Draw() {

	// Really... should go in Update().
	const int box_x1 = box_margin;
	const int box_x2 = WINDOW->Width() - (box_margin * 2);

	// Top of screen:
	const int box_y1 = box_margin; 
	const int box_y2 = box_margin + box_height;

	// or, Bottom of screen:
	// const int box_y1 = WINDOW->Height() - (box_margin + box_height);
	// const int box_y2 = WINDOW->Height() - box_margin;

	int txt_x;
	int txt_y;

	/* draw underlying box */
	WINDOW->DrawRect(	box_x1,	box_y1, 
										box_x2,	box_y2,
										box_color, true);

	// - draw avatar -
	if (avatar_sprite)
		WINDOW->DrawSprite(avatar_sprite, box_x1, box_y1);

	// - draw text -
	txt_x = box_x1 + box_margin;
	txt_y = box_y1 + box_margin;

	if (avatar_sprite)
		txt_x += avatar_sprite->width;

	std::string text = page_texts[text_index];


	time_until_next_blink--;

	if (time_until_next_blink == 0) {
		blink = !blink;

		// make the cursor blink a little longer when it's on.
		if (blink)
			time_until_next_blink = int(float(BLINK_TIME * 1.5f));
		else
			time_until_next_blink = BLINK_TIME;
	}

	if (wait_sprite && blink) {
		WINDOW->DrawSprite(wait_sprite, 
				box_x2 - box_margin - wait_sprite->width, 
				box_y2 - box_margin - wait_sprite->height
			);
	}

	if (text.length() > 0)
		WINDOW->DrawText(txt_x, txt_y, text);
}

void ObjectText::Update() {
	BaseUpdate();	
	
	// if we're modal, NOTHING else can happen
	// until the user presses a key
	if (is_modal) {

		/*if (INPUT->KeyOnce(PLAYERKEY_ACTION1, 1)) {
			SetModalActive(false);
			is_dead = true;
		} else */if (INPUT->KeyOnce(PLAYERKEY_JUMP, 1)) {
			
			++text_index;		// go to the next page

			// If no more pages...
			if (text_index >= page_texts.size()) {
				SetModalActive(false);
				is_dead = true;
			}
		}

		if (is_dead)
			EVENTS->OnTextboxDone();
	}
}

#define OBJECT_TXT_PAGE_DELIM "~"			// Which char goes to the next page

void ObjectText::SetText(std::string txt) {
	page_texts.clear();
	text_index = 0;
	
	if (txt.length() <= 0) {
		txt = "-INVALID TEXT-";
		page_texts.push_back(txt);
		return;
	}

	// Split up the pages based on the delimiter (currently a tilda "~")
	StringSplit(txt, OBJECT_TXT_PAGE_DELIM, page_texts);
}

void ObjectText::SetModalActive(bool state) {
	is_modal = state;

	if (is_modal)
		WORLD->SetModalObject(this);
	else
		WORLD->SetModalObject(NULL);
}

bool ObjectText::Init() {
	is_modal = false;
	avatar_sprite = NULL;
	box_margin 	= DEFAULT_BOX_MARGIN;
	box_color		=	DEFAULT_BOX_COLOR;
	box_height	= DEFAULT_BOX_HEIGHT;

	blink = false;
	time_until_next_blink = BLINK_TIME;

	properties.is_overlay = 1;
	properties.is_physical = 0;

	SetText("");
	SetAvatarFilename("");

	wait_sprite = ASSETMANAGER->LoadSprite("wait1.png", true);

	// really shouldn't assume this...
	SetModalActive(true);
	
	bool noError = BaseInit();

	// slight hack. if the previous mode requested no initial text
	// and we are initial text then die now.
	if (	noError && 
				WORLD->IsLoading() && 
				WORLD->GetOldExitInfo().useExitInfo && 
				!WORLD->GetOldExitInfo().showInitialText) {

		SetModalActive(false);
		is_dead = true;

		Update();
	}

	return noError;
}

bool ObjectText::SetAvatarFilename(std::string file) {
	if (file.length() <= 0)
		return false;

	avatar_sprite = ASSETMANAGER->LoadSprite(file.c_str());
	
	if (!avatar_sprite)
		return false;

	avatar_sprite->x_offset = 5;
	avatar_sprite->y_offset = 5;

	return true;
}

void ObjectText::Shutdown() {
	text_index = 0;
	page_texts.clear();
	avatar_sprite = NULL;

	BaseShutdown();
}

void ObjectText::Clear() {
	is_modal = false;
	avatar_sprite = NULL;
}

ObjectText::ObjectText() {
	Clear();
}

ObjectText::~ObjectText() {
}

BOOST_CLASS_EXPORT_GUID(ObjectText, "ObjectText")