#include "stdafx.h"
#include "gameMode.h"

bool GameMode::Init(XMLNode)
{
	return false;
}

GameMode::GameMode() {}
GameMode::~GameMode() {}

BOOST_CLASS_EXPORT_GUID(GameMode, "GameMode")