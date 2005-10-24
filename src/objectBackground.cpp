#include "objectBackground.h"

void BackgroundObject::Update() {
}

bool BackgroundObject::Init(GameState *_game_state) {
		SetGameState(_game_state);
		return true;
}

void BackgroundObject::Shutdown() {
}

void BackgroundObject::Draw() {
	DrawAtOffset(0, 0);
}

BackgroundObject::BackgroundObject() {}
BackgroundObject::~BackgroundObject() {}
