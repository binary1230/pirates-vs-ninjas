#include "credits.h"
#include "gameState.h"
#include "input.h"
#include "assetManager.h"
#include "window.h"

int CreditsMode::Init(GameState* gs, XMLNode xMode) {
	SetGameState(gs);

	AssetManager* m = GetGameState()->GetAssetManager();
	Window* window = GetGameState()->GetWindow();

	credits_bmp = m->LoadBitmap(xMode.getChildNode("scrollPic").getText());

	scroll_speed = xMode.getChildNode("scrollSpeed").getInt();
	scroll_offset = window->Height();

	window->SetClearColor(0);

	if (!credits_bmp)
		return -1;

	return 0;
}

void CreditsMode::Shutdown() {
	SetGameState(NULL);
}

void CreditsMode::Draw() {
	Window* window = GetGameState()->GetWindow();

	// XXX Should use window->Blit()...
	window->DrawBitmap(	credits_bmp, 
											window->Width()/2 - credits_bmp->w/2, 
											scroll_offset );
}

void CreditsMode::Update() {
	GameState* gs = GetGameState();

	scroll_offset -= scroll_speed;

	// If we finished scrolling or they press the exit key, we exit
	if (scroll_offset < -int(credits_bmp->h)
			|| gs->GetKey(GAMEKEY_EXIT)) {
		    GetGameState()->SignalEndCurrentMode();
	}
}

CreditsMode::CreditsMode() {
	SetGameState(NULL);
	credits_bmp = NULL;
}

CreditsMode::~CreditsMode() {
	SetGameState(NULL);
	credits_bmp = NULL;
}
