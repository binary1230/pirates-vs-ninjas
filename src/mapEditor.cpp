#include "mapEditor.h"
#include "mapSaver.h"
#include "sprite.h"
#include "window.h"
#include "assetManager.h"
#include "input.h"
#include "gameState.h"
#include "window.h"

MapEditor::MapEditor() {}
MapEditor::~MapEditor() {}

int MapEditor::Init(XMLNode xMode) {
	int ret_val = GameWorld::Init(xMode);

	if (ret_val == -1)
		return -1;
		
	MapSaver mapSaver;
	mapSaver.SaveEverything(this, "test-map.xml", xObjDefs);

	cursor_sprite = ASSETMANAGER->LoadSprite("cursor.png", true);
	assert(cursor_sprite && "Unable to load cursor");

	// Makes all layers scroll at the same rate instead of in parallax
	use_scroll_speed = false;

	selection = NULL;

	return ret_val;
}

void MapEditor::Select(Object* obj) {
	selection = obj;
}

void MapEditor::Shutdown() {
	GameWorld::Shutdown();
}

void MapEditor::Draw() {
	GameWorld::Draw();

	// Draw the cursor
	WINDOW->DrawSprite(cursor_sprite, INPUT->MouseX(), INPUT->MouseY());
}

#define SCROLL_VALUE 10

void MapEditor::ComputeNewScrolling() {	
	// compute new camera
	if (key[KEY_RIGHT])
		camera_x += SCROLL_VALUE;
	if (key[KEY_LEFT])
		camera_x -= SCROLL_VALUE;
	if (key[KEY_UP])
		camera_y += SCROLL_VALUE;
	if (key[KEY_DOWN])
		camera_y -= SCROLL_VALUE;

	 // keep it from getting off screen
	 if (camera_x < 0) 
			camera_x = 0;
	 if (camera_x > width - (int)WINDOW->Width()) 
			camera_x = width - WINDOW->Width();
	 if (camera_y < 0) 
			camera_y = 0;
	 if (camera_y > height - (int)WINDOW->Height()) 
			camera_y = height - WINDOW->Height();
}

void MapEditor::UpdateIdle() {
	
}

void MapEditor::Update() {
	if (INPUT->KeyOnce(GAMEKEY_EXIT)) {
		GAMESTATE->SignalGameExit();      // for real
		return;
	}

	ComputeNewScrolling();
}

/*void MapEditor::UpdateSelectMode() {
	ObjectListIter iter;
	Object* obj;

	// slow. don't do this.
	for (iter = objects.begin(); iter != objects.end(); iter++) {
		obj = *iter;
		if (obj->
	}
}*/

void MapEditor::LoadMusic(const char* music_file) {
	// don't load the music for the map editor, do nothing.
}

int MapEditor::LoadObjectDefsFromXML(XMLNode& _xObjDefs) {
	// save a copy
	xObjDefs = _xObjDefs;

	// call the original
	return GameWorld::LoadObjectDefsFromXML(_xObjDefs);
}
