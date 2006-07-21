#include "gameSound.h"
#include "gameState.h"
#include "resourceLoader.h"

GameSound::GameSound() {
	sound_enabled = false;
	game_state = NULL;
}

GameSound::~GameSound() {}

//! Plays a sound
//! Pan goes from 0-255, 128 being the center
void GameSound::PlaySound(unsigned int which_sound, unsigned int pan) {
	if (!sound_enabled)
		return;
	
	assert(which_sound < sounds.size());
	
	SAMPLE* spl = sounds[which_sound];

	if (spl)
		play_sample(spl, 255, pan, 1000, 0);
}

//! Loads a sound, returns a 'handle' to it that you can 
//! pass to PlaySound()
int GameSound::LoadSound(const char* filename) {
	if (!sound_enabled)
		return 0;

	SAMPLE* spl = game_state->GetResourceLoader()->LoadSound(filename);

	if (!spl)
		return -1;

	int i = sounds.size();
	sounds.resize(i+1);
	sounds[i] = spl;

	return i;
}
	
int GameSound::Init(GameState* _game_state, bool _sound_enabled) {
	sound_enabled = _sound_enabled;
	game_state = _game_state;

	if (!sound_enabled)
		return 0;	
	
	install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);

	sounds.clear();

	return 0;
}

void GameSound::Shutdown() {

	int i, max = sounds.size();
	game_state = NULL;
				
	if (!sound_enabled)
		return;

	for (i=0; i<max; i++) {
		if (sounds[i]) {
			destroy_sample(sounds[i]);
			sounds[i] = NULL;
		}
	}
	
	sounds.clear();
				
	sound_enabled = false;
	
}
