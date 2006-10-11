#include "credits.h"
#include "gameState.h"
#include "input.h"
#include "assetManager.h"
#include "window.h"
#include "gameSound.h"

int CreditsMode::Init(XMLNode xMode) {
	credits_bmp = ASSETMANAGER->LoadBitmap(
		xMode.getChildNode("scrollPic").getText()
	);

	if (!xMode.getChildNode("scrollSpeed").getInt(scroll_speed)) {
		fprintf(stderr, " -- Invalid scroll speed!\n");
		return -1;
	}
	scroll_offset = WINDOW->Height();

	WINDOW->SetClearColor(0);

	if (!credits_bmp)
		return -1;
	
	// Load the music
	if (xMode.nChildNode("music") == 1) {
		const char* music_file = xMode.getChildNode("music").getText();
		SOUND->LoadMusic(music_file);
		SOUND->PlayMusic();
	}

	return 0;
}

void CreditsMode::Shutdown() {
}

void CreditsMode::Draw() {
	// XXX Should use window->Blit()...
	WINDOW->DrawBitmap(	credits_bmp, 
											WINDOW->Width()/2 - credits_bmp->w/2, 
											scroll_offset );
}

void CreditsMode::Update() {

	// Hehehe.. weird scrolling hacks, try it.
	if (INPUT->Key(PLAYERKEY1_DOWN))
		scroll_offset+=10;
	else if (INPUT->Key(PLAYERKEY1_UP))
		scroll_offset-=10;
	else
		scroll_offset -= scroll_speed;		// NORMAL

	// If we finished scrolling or they press the exit key, we exit
	if (	scroll_offset < -int(credits_bmp->h) || INPUT->KeyOnce(GAMEKEY_EXIT)) {
   	GAMESTATE->SignalEndCurrentMode();
	}
}

CreditsMode::CreditsMode() {
	credits_bmp = NULL;
}

CreditsMode::~CreditsMode() {
	Shutdown();
}
