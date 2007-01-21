#include "gameSound.h"
#include "gameState.h"
#include "assetManager.h"
#include "oggFile.h"
#include "globalDefines.h"

DECLARE_SINGLETON(GameSound)

//! Plays a sound
//! Pan goes from 0-255, 128 being the center
void GameSound::PlaySound(CString name, unsigned int pan) {
	if (!sound_enabled)
		return;
	
	// modifying the pitch randomly produces weird variations
	// for the same sound.  slight pitch changes == good
	int freq = 1000;  							// pitch to play at (1000=normal)

	if (use_variable_pitch)
		freq += Rand(0, freq_range) - (freq_range / 2);
	
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

bool GameSound::LoadMusic(const char* filename) {
	if (!sound_enabled)
		return true;		

	OGGFILE* music = ASSETMANAGER->LoadMusic(filename);

	if (!music) 
		return false;
	else
		return true;
}

bool GameSound::PlayMusic(bool loop, int vol, int pan, int buflen) {
	if (!sound_enabled)
		return true;		

	OGGFILE* music = ASSETMANAGER->GetMusic();	

	if (!music)
		return false;
	else
		return music->Play(loop, vol, pan, buflen);
}

void GameSound::Update() {
	if (!sound_enabled)
		return;		
	
	OGGFILE* music = ASSETMANAGER->GetMusic();	

	if (music)
		music->Update();
}

//! Loads a sound, you can call it later with PlaySound(sound_name)
bool GameSound::LoadSound(const char* filename, const char* sound_name) {
	if (!sound_enabled)
		return true;

	SAMPLE* spl = ASSETMANAGER->LoadSound(filename);

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

int GameSound::Init(bool _sound_enabled) {
	sound_enabled = _sound_enabled;

	if (!sound_enabled)
		return 0;	
	
	//if (install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL) == -1) {
	if (install_sound(DIGI_AUTODETECT, 0, NULL) == -1) {
		fprintf(stderr, " WARNING: Sound init failure.  Message from Allegro:\n"
										"%s\n", allegro_error);
		sound_enabled = false;
	}

	sounds.clear();
	
	set_volume_per_voice(0);

	assert(GLOBALS->Value("sound_use_variable_pitch", use_variable_pitch));
	assert(GLOBALS->Value("sound_freq_range", freq_range));

	return 0;
}

void GameSound::Shutdown() {
	// Do NOT free any pointers in here
	// The actual memory for sounds and music 
	// is in the AssetManager
	sounds.clear();

	OGGFILE* music = ASSETMANAGER->GetMusic();

	if (music) {
		music->Shutdown();
	}

	remove_sound();
	sound_enabled = false;
}

GameSound::GameSound() {
	sound_enabled = false;
}

GameSound::~GameSound() {}
