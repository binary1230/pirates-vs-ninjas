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
	for (uint i = 0; i < m_kLayers.size(); ++i) {
		
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

	m_iCurrentObjectDefinitionIndex = 0;
	m_pkSelectedObject = NULL;

	m_iGridResolution = 1;

	WINDOW->SetTitle("Pirates VS Ninjas Map Editor");
	SetFlashText("Pirates VS Ninjas Map Editor, press H for help.");

	return ret_val;
}

void MapEditor::Select(Object* obj) {
	selection = obj;
}

void MapEditor::Shutdown() 
{
	UnselectCurrentlySelectedObject();

	MapSaver mapSaver;
	mapSaver.SaveEverything(this, "test-map.xml", xObjDefs);

	GAMESTATE->ResetAccumulatedTime();
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
	if (key[KEY_RIGHT] || key[KEY_D])
		m_iCameraX += SCROLL_VALUE;
	if (key[KEY_LEFT]  || key[KEY_A])
		m_iCameraX -= SCROLL_VALUE;
	if (key[KEY_UP] || key[KEY_W])
		m_iCameraY += SCROLL_VALUE;
	if (key[KEY_DOWN] || key[KEY_S])
		m_iCameraY -= SCROLL_VALUE;

	 // keep it from getting off screen
	if (m_iCameraX < 0) 
		m_iCameraX = 0;
	if (m_iCameraX > m_iLevelWidth - (int)WINDOW->Width()) 
		m_iCameraX = m_iLevelWidth - WINDOW->Width();
	if (m_iCameraY < 0) 
		m_iCameraY = 0;
	if (m_iCameraY > m_iLevelHeight - (int)WINDOW->Height()) 
		m_iCameraY = m_iLevelHeight - WINDOW->Height();
}

void MapEditor::ToggleOneLayerDisplay() 
{
	m_bDisplayOneLayerOnly = !m_bDisplayOneLayerOnly;

	if (m_bDisplayOneLayerOnly)
		SetFlashText("Single layer display ON.");
	else
		SetFlashText("Single layer display OFF.");

	for (uint i = 0; i < m_kLayers.size(); ++i) {
		m_kLayers[i]->SetVisible(!m_bDisplayOneLayerOnly);
	}

	if (m_bDisplayOneLayerOnly && m_iCurrentLayer >= 0 && m_iCurrentLayer < (int)m_kLayers.size())
		m_kLayers[m_iCurrentLayer]->SetVisible(true);
}

void MapEditor::SelectNextLayer()
{
	if (m_bDisplayOneLayerOnly)
		m_kLayers[m_iCurrentLayer]->SetVisible(false);

	m_iCurrentLayer++;
	if (m_iCurrentLayer == (int)m_kLayers.size())
		m_iCurrentLayer = 0;

	if (m_bDisplayOneLayerOnly)
		m_kLayers[m_iCurrentLayer]->SetVisible(true);

	SetFlashText("Selecting layer: %s", m_kLayers[m_iCurrentLayer]->GetName());
}

void MapEditor::SelectPreviousLayer()
{
	if (m_bDisplayOneLayerOnly)
		m_kLayers[m_iCurrentLayer]->SetVisible(false);

	m_iCurrentLayer--;
	if (m_iCurrentLayer == -1)
		m_iCurrentLayer = m_kLayers.size() - 1;

	if (m_bDisplayOneLayerOnly)
		m_kLayers[m_iCurrentLayer]->SetVisible(true);

	SetFlashText("Selecting layer: %s", m_kLayers[m_iCurrentLayer]->GetName());
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
	if (OBJECT_FACTORY->GetObjectDefinitionCount() == 0) 
	{
		m_eCurrentMode = MODE_MAIN;
		m_iCurrentObjectDefinitionIndex = -1;
		assert(!m_pkSelectedObject);
		return;
	}

	UpdateSelectedObjectPosition();
	UpdateSelectedObjectLayer();

	// insert it permanently into world if they clicked
	if (INPUT->MouseButtonOnce(MOUSE_LEFT_BTN))
		UnselectCurrentlySelectedObject();

	UpdateCurrentObjectDefinitionIfNeeded();
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
	else if (INPUT->RealKeyOnce(KEY_PAUSE))
		m_kMapEditorGui.ReloadLuaScript();
	else if (INPUT->RealKeyOnce(KEY_G))
	{
		if (m_iGridResolution == 1)
			m_iGridResolution = 16;
		else
			m_iGridResolution = 1;
	}

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

void MapEditor::RunMapEditor()
{
	m_kMapEditorGui.Start();
}

void MapEditor::RemoveCurrentlySelectedObject()
{
	if (!m_pkSelectedObject)
		return;

	m_pkSelectedObject->SetIsDead(true);
	m_pkSelectedObject = NULL;
	RemoveDeadObjectsIfNeeded();
}

void MapEditor::UpdateSelectedObjectPosition()
{
	if (!m_pkSelectedObject)
		return;

	m_pkSelectedObject->SetDrawBounds(true);

	assert(m_pkSelectedObject->GetLayer());

	int x = (int)(INPUT->MouseX() / m_pkSelectedObject->GetLayer()->GetScrollSpeed()) + m_iCameraX;
	int y = (int)((WINDOW->Height() - INPUT->MouseY()) / m_pkSelectedObject->GetLayer()->GetScrollSpeed()) + m_iCameraY;

	if (m_iGridResolution != 1)
	{
		x -= x % m_iGridResolution;
		y -= y % m_iGridResolution;
	}

	m_pkSelectedObject->SetXY(x, y);
}

void MapEditor::UnselectCurrentlySelectedObject()
{
	if (!m_pkSelectedObject)
		return;

	m_pkSelectedObject->SetDrawBounds(false);

	// just remove our reference to it, it's already inserted into the world.
	m_pkSelectedObject = NULL;
}

void MapEditor::AddNewObjectToWorld( int iObjectDefinitionIndexToAdd )
{
	const CString& sObjectName = OBJECT_FACTORY->GetObjectDefinition(iObjectDefinitionIndexToAdd);
	SetFlashText("OBJECTS: Looking at object: '%s'", sObjectName.c_str());

	m_pkSelectedObject = OBJECT_FACTORY->CreateObject(sObjectName);
	assert(m_pkSelectedObject);

	m_pkSelectedObject->SetLayer(m_kLayers[m_iCurrentLayer]);
	WORLD->AddObject(m_pkSelectedObject, true);
}

void MapEditor::UpdateCurrentObjectDefinitionIfNeeded()
{
	bool bGetPrevious = INPUT->RealKeyOnce(KEY_INSERT);
	bool bGetNext = INPUT->RealKeyOnce(KEY_DEL);

	// Do nothing if there's no input or no selection
	if (m_pkSelectedObject && !bGetNext && !bGetPrevious)
		return;

	// To move up and down the object type list, we
	// delete the object that is selected, and re-add a new object
	// with the new selected object definition index we compute
	// based on user input.

	RemoveCurrentlySelectedObject();

	if (bGetNext) {
		m_iCurrentObjectDefinitionIndex++;
		if (m_iCurrentObjectDefinitionIndex >= OBJECT_FACTORY->GetObjectDefinitionCount())
			m_iCurrentObjectDefinitionIndex = 0;
	}

	if (bGetPrevious) {
		m_iCurrentObjectDefinitionIndex--;
		if (m_iCurrentObjectDefinitionIndex < 0)
			m_iCurrentObjectDefinitionIndex = OBJECT_FACTORY->GetObjectDefinitionCount() - 1;
	}

	AddNewObjectToWorld(m_iCurrentObjectDefinitionIndex);
}

void MapEditor::UpdateSelectedObjectLayer()
{
	if (!m_pkSelectedObject || m_pkSelectedObject->GetLayer() == m_kLayers[m_iCurrentLayer])
		return;
	
	// TODO: This should just be one call to SetLayer(), refactor this junk in there.
	m_pkSelectedObject->GetLayer()->RemoveObject(m_pkSelectedObject);
	m_pkSelectedObject->SetLayer(m_kLayers[m_iCurrentLayer]);
	m_pkSelectedObject->GetLayer()->AddObject(m_pkSelectedObject);
}