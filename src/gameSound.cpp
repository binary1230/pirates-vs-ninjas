#include "gameSound.h"
#include "gameState.h"
#include "assetManager.h"

GameSound::GameSound() {
	sound_enabled = false;
	game_state = NULL;
}

GameSound::~GameSound() {}

//! Plays a sound
//! Pan goes from 0-255, 128 being the center
void GameSound::PlaySound(CString name, unsigned int pan) {

	const int freq = 1000;  	// pitch to play at (1000=normal)
	
	if (!sound_enabled)
		return;
	
	s_iter s = sounds.find(name.c_str());

	if (s == sounds.end()) {
		fprintf(stderr, "- sound: warning: Sound '%s' was never loaded.\n", 
										name.c_str());
		return;
	}
	
	SAMPLE* spl = s->second;

	if (spl)
		play_sample(spl, 255, pan, freq, 0);
}

//! Loads a sound, you can call it later with PlaySound(sound_name)
bool GameSound::LoadSound(const char* filename, const char* sound_name) {
	if (!sound_enabled)
		return true;

	SAMPLE* spl = game_state->GetAssetManager()->LoadSound(filename);

	if (!spl)
		return false;
	
	sounds[sound_name] = spl;
	return true;
}
	
bool GameSound::LoadSounds(XMLNode &xSounds) {
	XMLNode xSound;
	int max, i, iterator;

	max = xSounds.nChildNode("sound");
	for (i = iterator = 0; i<max; i++) {
		xSound = xSounds.getChildNode("sound", &iterator);
		CString name = xSound.getAttribute("name");
		
		if (LoadSound(xSound.getText(), name) == -1) {
			fprintf(stderr, "ERROR: Can't load soundfile: '%s'\n", 
											xSound.getText());
			return false;
		}
	}
	
	return true;
}

int GameSound::Init(GameState* _game_state, bool _sound_enabled) {
	sound_enabled = _sound_enabled;
	game_state = _game_state;

	if (!sound_enabled)
		return 0;	
	
	//if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) == -1) {
	if (install_sound(DIGI_AUTODETECT, 0, NULL) == -1) {
		fprintf(stderr, " WARNING: Sound init failure.  Message from Allegro:\n"
										"%s\n", allegro_error);
		sound_enabled = false;
	}

	sounds.clear();

	return 0;
}

void GameSound::Shutdown() {
	s_iter s;
	game_state = NULL;
				
	// do NOT free pointers in here
	sounds.clear();
	sound_enabled = false;

	remove_sound();
}
