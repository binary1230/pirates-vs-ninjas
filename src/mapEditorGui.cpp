#include "stdafx.h"
#include "mapEditor.h"
#include "mapEditorGui.h"
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

// gui stuff
#include "GUI_layeroptions.h"

void MapEditorGui::Shutdown()
{

}

bool MapEditorGui::UpdateFltk() {
	Fl::wait(0.0);
	return true;
}

void MapEditorGui::Update()
{
	UpdateFltk();
}

bool MapEditorGui::Init() 
{
	m_pkWindow = FLTK_DisplayLayerOptions();

	UpdateFltk();

	// HACK: Force the window to show up
	// For some reason under win32 + msvc, the main window always appears
	// on top of this window no matter what.  
	// Here, we minimze then maximize it so it shows up first
	m_pkWindow->iconize();
	m_pkWindow->show();
	m_pkWindow->redraw();

	return true;
}

MapEditorGui::MapEditorGui()  {}
MapEditorGui::~MapEditorGui() {}