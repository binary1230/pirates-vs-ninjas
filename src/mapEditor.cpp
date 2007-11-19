#include "stdafx.h"
#include "mapEditor.h"
#include "objectLayer.h"
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

	m_iCurrentLayer = 0;
	m_bDisplayOneLayerOnly = 0;
	m_uiTxtTicksLeft = 0;

	WINDOW->SetTitle("Pirates VS Ninjas Map Editor");
	SetFlashText("Pirates VS Ninjas Map Editor, press H for help.");

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

	if (m_uiTxtTicksLeft > 0)
		WINDOW->DrawText(10, 10, m_sTxt);

	// Draw the cursor
	WINDOW->DrawSprite(cursor_sprite, INPUT->MouseX(), INPUT->MouseY());
}

void MapEditor::SetFlashText(char * format, ... )
{
	va_list args;
	const int bufsize = 4000;
	static char buffer[bufsize];

	va_start( args, format );
	vsnprintf( buffer, bufsize - 1, format, args );

	m_sTxt = buffer;
	m_uiTxtTicksLeft = 60;
}

#define SCROLL_VALUE 30

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

void MapEditor::ToggleOneLayerDisplay() 
{
	m_bDisplayOneLayerOnly = !m_bDisplayOneLayerOnly;

	if (m_bDisplayOneLayerOnly)
		SetFlashText("Single layer display ON.");
	else
		SetFlashText("Single layer display OFF.");

	for (uint i = 0; i < layers.size(); ++i) {
		layers[i]->SetVisible(!m_bDisplayOneLayerOnly);
	}

	if (m_bDisplayOneLayerOnly && m_iCurrentLayer >= 0 && m_iCurrentLayer < (int)layers.size())
		layers[m_iCurrentLayer]->SetVisible(true);
}

void MapEditor::SelectNextLayer()
{
	if (!m_bDisplayOneLayerOnly)
		return;

	layers[m_iCurrentLayer]->SetVisible(false);

	m_iCurrentLayer++;
	if (m_iCurrentLayer == layers.size())
		m_iCurrentLayer = 0;

	layers[m_iCurrentLayer]->SetVisible(true);

	SetFlashText("Selecting layer: %s", layers[m_iCurrentLayer]->GetName());
}

void MapEditor::SelectPreviousLayer()
{
	if (!m_bDisplayOneLayerOnly)
		return;

	layers[m_iCurrentLayer]->SetVisible(false);

	m_iCurrentLayer--;
	if (m_iCurrentLayer == -1)
		m_iCurrentLayer = layers.size() - 1;

	layers[m_iCurrentLayer]->SetVisible(true);

	SetFlashText("Selecting layer: %s", layers[m_iCurrentLayer]->GetName());
}

void MapEditor::CheckForInput()
{
	INPUT->Update();

	if (INPUT->KeyOnce(GAMEKEY_EXIT)) {
		GAMESTATE->SignalGameExit();      // for real
		return;
	}

	if (INPUT->RealKeyOnce(KEY_1))
		ToggleOneLayerDisplay();
	else if (INPUT->RealKeyOnce(KEY_PGUP))
		SelectNextLayer();
	else if (INPUT->RealKeyOnce(KEY_PGDN))
		SelectPreviousLayer();
}

void MapEditor::Update() {
	CheckForInput();
	ComputeNewScrolling();

	if (m_uiTxtTicksLeft > 0)
		m_uiTxtTicksLeft--;
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
