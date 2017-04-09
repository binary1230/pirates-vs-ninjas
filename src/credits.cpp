#include "stdafx.h"
#include "credits.h"
#include "gameState.h"
#include "input.h"
#include "assetManager.h"
#include "window.h"
#include "gameSound.h"

int CreditsMode::Init(XMLNode xMode) {
	CString file = xMode.getChildNode("scrollPic").getText();
	
	credits_sprite = ASSETMANAGER->LoadSprite(file.c_str());

	if (!credits_sprite)
		return -1;

	if (!xMode.getChildNode("scrollSpeed").getInt(scroll_speed)) {
		TRACE(" -- Invalid scroll speed!\n");
		return -1;
	}
	scroll_offset = WINDOW->Height();

	WINDOW->SetClearColor(0,0,0);
	
	// Load the music
	if (xMode.nChildNode("music") == 1) {
		const char* music_file = xMode.getChildNode("music").getText();
		SOUND->PlayMusic(music_file);
	}

	return 0;
}

void CreditsMode::Shutdown() {
}

void CreditsMode::Draw() {
	// XXX Should use window->Blit()...
	WINDOW->DrawSprite(	credits_sprite, 
											WINDOW->Width()/2 - credits_sprite->width/2, 
											scroll_offset );
}

void CreditsMode::Update() {


	// Hehehe.. weird scrolling hacks, try it.
	if (INPUT->Key(PLAYERKEY1_DOWN))
		scroll_offset+=10;
	else if (INPUT->Key(PLAYERKEY1_UP))
		scroll_offset-=10;

	// Do it normally, if they're not pressing the JUMP key
	else if (!INPUT->Key(PLAYERKEY1_JUMP))
		scroll_offset -= scroll_speed;		// NORMAL

	// If we finished scrolling or they press the exit key, we exit
	if (	scroll_offset < -int(credits_sprite->height) || 
				INPUT->KeyOnce(GAMEKEY_EXIT))	{
   	GAMESTATE->SignalEndCurrentMode();
	}
}

CreditsMode::CreditsMode() {
	credits_sprite = NULL;
}

CreditsMode::~CreditsMode() {
	Shutdown();
}
