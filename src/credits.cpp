#include "credits.h"
#include "gameState.h"
#include "input.h"
#include "assetManager.h"
#include "window.h"
#include "gameSound.h"

int CreditsMode::Init(GameState* gs, XMLNode xMode) {
	SetGameState(gs);

	AssetManager* m = GetGameState()->GetAssetManager();
	Window* window = GetGameState()->GetWindow();

	credits_bmp = m->LoadBitmap(xMode.getChildNode("scrollPic").getText());

	if (!xMode.getChildNode("scrollSpeed").getInt(scroll_speed)) {
		fprintf(stderr, " -- Invalid scroll speed!\n");
		return -1;
	}
	scroll_offset = window->Height();

	window->SetClearColor(0);

	if (!credits_bmp)
		return -1;
	
	// Load the music
	if (xMode.nChildNode("music") == 1) {
		const char* music_file = xMode.getChildNode("music").getText();
		GameSound* sound = GetGameState()->GetSound();
		sound->LoadMusic(music_file);
		sound->PlayMusic();
	}

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
			|| gs->GetInput()->KeyOnce(GAMEKEY_EXIT)) {
		    GetGameState()->SignalEndCurrentMode();
	}
}

CreditsMode::CreditsMode() {
	SetGameState(NULL);
	credits_bmp = NULL;
}

CreditsMode::~CreditsMode() {
	Shutdown();
}
