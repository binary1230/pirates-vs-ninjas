#include "stdafx.h"
#include "gameMode.h"
#include "game.h"

bool GameMode::Init(XMLNode)
{
	GAME->CreateGameStateIfNotExists();

	return true;
}

GameMode::GameMode() {}
GameMode::~GameMode() {}

BOOST_CLASS_EXPORT_GUID(GameMode, "GameMode")