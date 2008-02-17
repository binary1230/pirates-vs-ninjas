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
#include "luaManager.h"

void MapEditorGui::Shutdown()
{
	LUA->RunVoidFunctionNoArgs("shutdown", true);
	LUA->ReleaseCurrentLuaScript();
}

void MapEditorGui::Update()
{
	LUA->RunVoidFunctionNoArgs("update", true);
}

bool MapEditorGui::Init() 
{
	LUA->LoadLuaScript("src/lua/mapeditorGUI.lua");
	LUA->RunVoidFunctionNoArgs("init", true);

	return true;
}

MapEditorGui::MapEditorGui()  {}
MapEditorGui::~MapEditorGui() {}
