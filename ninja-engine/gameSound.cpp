#include "stdafx.h"
#include "gameOptions.h"
#include "gameSound.h"
#include "gameState.h"
#include "assetManager.h"
#include "globalDefines.h"

DECLARE_SINGLETON(GameSound)

ALLEGRO_SAMPLE* GameSound::FindCachedSoundByName(const char* name) {
	auto s = soundMap.find(name);
	if (s == soundMap.end())
		return NULL;

	return s->second;
}

//! Plays a sound
// TODO: Pan is broken, should be a float -2017
void GameSound::PlaySound(std::string name, unsigned int pan, ALLEGRO_PLAYMODE loop, ALLEGRO_SAMPLE_ID* sound_id_out) {
	if (!sound_enabled)
		return;
	
	ALLEGRO_SAMPLE* spl = NULL;
	spl = FindCachedSoundByName(name.c_str());

	if (!spl) {
		spl = LoadSound(name.c_str(), name.c_str());
	}

	if (!spl) {
		TRACE("- SOUND: ERROR: Can't load [non-cached] sound '%s'\n", name);
		return;
	}
	
	al_play_sample(spl, 1.0f, ALLEGRO_AUDIO_PAN_NONE, 1.0f, loop, sound_id_out);
}

bool GameSound::IsThisMusicPlayingAlready(std::string name) {
	return current_music.length() && current_music == name;
}

bool GameSound::PlayMusic(const char* name) {
	if (!sound_enabled)
		return true;

	if (IsThisMusicPlayingAlready(name))
		return true;

	StopMusic();
	PlaySound(name, 0, ALLEGRO_PLAYMODE_LOOP, &current_music_id);
	current_music = name;

	return true;
}

void GameSound::StopMusic() {
	if (!current_music.length())
		return;
	
	StopSound(&current_music_id);
	current_music = "";
}

void GameSound::StopSound(ALLEGRO_SAMPLE_ID* sample_id) {
	if (sample_id)
		al_stop_sample(sample_id);
}

void GameSound::Update() {

}

//! Loads a sound, you can call it later with PlaySound(sound_name)
ALLEGRO_SAMPLE* GameSound::LoadSound(const char* filename, const char* sound_name) {
	if (!sound_enabled)
		return NULL;

	// we need to enable this sometime, we should not be overwriting stuff 
	// that's already in the map, that should be an error.
	// right now am too lazy to deal with this properly.
	// assert(!FindCachedSoundByName(sound_name));

	ALLEGRO_SAMPLE* spl = ASSETMANAGER->LoadSound(filename);

	if (!spl)
		return NULL;
	
	soundMap[sound_name] = spl;
	return spl;
}
	
bool GameSound::LoadSounds(XMLNode &xSounds) {
	XMLNode xSound;
	int max, i, iterator;

	max = xSounds.nChildNode("sound");
	for (i = iterator = 0; i<max; i++) {
		xSound = xSounds.getChildNode("sound", &iterator);
		std::string name = xSound.getAttribute("name");
		
		if (!LoadSound(xSound.getText(), name.c_str())) {
			TRACE("ERROR: Can't load soundfile: '%s'\n", xSound.getText());
			return false;
		}
	}
	
	return true;
}

int GameSound::Init(bool _sound_enabled) {
	sound_enabled = _sound_enabled;

	if (!sound_enabled)
		return 0;	

	current_music = "";
	
	if (!al_install_audio()) {
		TRACE(" WARNING: Sound init failure.  Message from Allegro:\n%s\n", /*allegro_error*/ "");
		sound_enabled = false;
	}

	if (!al_init_acodec_addon()) {
		TRACE("failed to initialize audio codecs!\n");
		return -1;
	}

	if (!al_reserve_samples(10)) {
		fprintf(stderr, "failed to reserve samples!\n");
		return -1;
	}

	soundMap.clear();

	GLOBALS->Value("sound_use_variable_pitch", use_variable_pitch);
	GLOBALS->Value("sound_freq_range", freq_range);

	return 0;
}

void GameSound::Shutdown() {
	StopMusic();

	// Do NOT free any pointers in here
	// The actual memory for sounds and music 
	// is in the AssetManager
	soundMap.clear();

	sound_enabled = false;

	al_uninstall_audio();
}

GameSound::GameSound() {
	sound_enabled = false;
}

GameSound::~GameSound() {}

void GameSound::ClearSoundMap() {
	soundMap.clear();
}
