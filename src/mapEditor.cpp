#include "stdafx.h"
#include "luaManager.h"
#include "mapEditor.h"
#include "objectLayer.h"
#include "mapSaver.h"
#include "sprite.h"
#include "window.h"
#include "assetManager.h"
#include "input.h"
#include "gameState.h"
#include "window.h"
#include "object.h"
#include "objectFactory.h"

MapEditor::MapEditor() {}
MapEditor::~MapEditor() {}

void MapEditor::RePopulateLayerList() 
{
	for (uint i = 0; i < layers.size(); ++i) {
		
	}
}

int MapEditor::Init(XMLNode xMode) {
	if (!m_kMapEditorGui.Init())
	 	return -1;

	int ret_val = GameWorld::Init(xMode);

	if (ret_val == -1)
		return -1;

	cursor_sprite = ASSETMANAGER->LoadSprite("cursor.png", true);
	assert(cursor_sprite && "Unable to load cursor");

	// Makes all layers scroll at the same rate instead of in parallax
	//use_scroll_speed = false;

	selection = NULL;

	m_iCurrentLayer = 0;
	m_bDisplayOneLayerOnly = 0;
	m_uiTxtTicksLeft = 0;
	m_eCurrentMode = MODE_MAIN;

	WINDOW->SetTitle("Pirates VS Ninjas Map Editor");
	SetFlashText("Pirates VS Ninjas Map Editor, press H for help.");

	return ret_val;
}

void MapEditor::Select(Object* obj) {
	selection = obj;
}

void MapEditor::Shutdown() {
	MapSaver mapSaver;
	mapSaver.SaveEverything(this, "test-map.xml", xObjDefs);

	LUA->RunVoidFunctionNoArgs("shutdown", true);

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
	if (m_bDisplayOneLayerOnly)
		layers[m_iCurrentLayer]->SetVisible(false);

	m_iCurrentLayer++;
	if (m_iCurrentLayer == (int)layers.size())
		m_iCurrentLayer = 0;

	if (m_bDisplayOneLayerOnly)
		layers[m_iCurrentLayer]->SetVisible(true);

	SetFlashText("Selecting layer: %s", layers[m_iCurrentLayer]->GetName());
}

void MapEditor::SelectPreviousLayer()
{
	if (m_bDisplayOneLayerOnly)
		layers[m_iCurrentLayer]->SetVisible(false);

	m_iCurrentLayer--;
	if (m_iCurrentLayer == -1)
		m_iCurrentLayer = layers.size() - 1;

	if (m_bDisplayOneLayerOnly)
		layers[m_iCurrentLayer]->SetVisible(true);

	SetFlashText("Selecting layer: %s", layers[m_iCurrentLayer]->GetName());
}

void MapEditor::ModeUpdate()
{
	CommonModeUpdateStart();
	
	// Figure out if we go to a different mode now
	if (INPUT->RealKeyOnce(KEY_F1))
		m_eCurrentMode = MODE_OBJECT_PLACEMENT;
	
	switch (m_eCurrentMode) {
		default:
			assert(0 && "Invalid mode.");
			break;

		case MODE_MAIN:
			break; // DO nothing

		case MODE_OBJECT_PLACEMENT:
			ModeObjectPlacementUpdate();
			break;
	}

	CommonModeUpdateEnd();
}

void MapEditor::ModeObjectPlacementUpdate()
{
	static int iCurrentIndex = 0;
	static Object* pkObj = NULL;

	if (OBJECT_FACTORY->GetObjectDefinitionCount() == 0) {
		m_eCurrentMode = MODE_MAIN;
		iCurrentIndex = -1;
		return;
	}

	// -----------------------------------------------------------
	// move existing object
	// -----------------------------------------------------------

	if (pkObj) {

		int x = (int)(INPUT->MouseX() / pkObj->GetLayer()->GetScrollSpeed()) + camera_x;
		int y = (int)((WINDOW->Height() - INPUT->MouseY()) / pkObj->GetLayer()->GetScrollSpeed()) + camera_y;
		pkObj->SetXY(x, y);
		
		assert(pkObj->GetLayer());

		// TODO: This should just be one call to SetLayer(), refactor this junk in there.
		if (pkObj->GetLayer())
			pkObj->GetLayer()->RemoveObject(pkObj);
		pkObj->SetLayer(layers[m_iCurrentLayer]);
		pkObj->GetLayer()->AddObject(pkObj);

		// insert it permanently if they clicked
		if (INPUT->MouseButtonOnce(MOUSE_LEFT_BTN)) {
			pkObj = NULL;	// just remove our reference to it, it's already in the world.
		}
	}

	// -----------------------------------------------------------

	bool bGetPrevious = INPUT->RealKeyOnce(KEY_INSERT);
	bool bGetNext = INPUT->RealKeyOnce(KEY_DEL);

	if (pkObj && !bGetNext && !bGetPrevious)
		return;

	// -----------------------------------------------------------
	// create a new object/got to previous/next object
	// -----------------------------------------------------------

	if (bGetNext) {
		iCurrentIndex++;
		if (iCurrentIndex >= OBJECT_FACTORY->GetObjectDefinitionCount())
			iCurrentIndex = 0;
	}

	if (bGetPrevious) {
		iCurrentIndex--;
		if (iCurrentIndex < 0)
			iCurrentIndex = OBJECT_FACTORY->GetObjectDefinitionCount() - 1;
	}

	// delete the old object
	if (pkObj) {
		pkObj->SetIsDead(true);
		pkObj = NULL;
		DoCleaning();
	}

	// add the new object
	const CString& sObjectName = OBJECT_FACTORY->GetObjectDefinition(iCurrentIndex);
	SetFlashText("OBJECTS: Looking at object: '%s'", sObjectName.c_str());

	pkObj = OBJECT_FACTORY->CreateObject(sObjectName);
	assert(pkObj);

	pkObj->SetLayer(layers[m_iCurrentLayer]);
	WORLD->AddObject(pkObj, true);
}

void MapEditor::CommonModeUpdateStart() 
{
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

	ComputeNewScrolling();
}

void MapEditor::CommonModeUpdateEnd() 
{
	
}

void MapEditor::Update() {
	INPUT->Update();
	m_kMapEditorGui.Update();
	ModeUpdate();

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
