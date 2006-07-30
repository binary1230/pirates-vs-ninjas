#include "inputRecord.h"
#include "globals.h"
#include "gameState.h"

int InputRecord::Init(GameState* _game_state, CString _demo_file) {

	SetGameState(_game_state);
				
	install_mouse();
	install_keyboard();
		
	if (!BaseInit()) {
		return -1;
	}

	LoadDefaultKeyMappings();

	if (InitRecorder(_demo_file))
		return 0;
	else 
		return -1;
}

void InputRecord::Shutdown() {
	remove_mouse();
	remove_keyboard();

	if (demofile) {
		fprintf(stderr, "WARN: closing demofile (currently in progress)");
		fclose(demofile);
	}
}

//! Update the state of the input

//! This is A little complicated..
//! First we save the old keys being pressed.
//! Then we get the new keys being pressed.
//! If anything is different, write it to the demo with the frame counter
void InputRecord::Update() {
	
	uint i;
	bool keys_changed = false;
	
	// TODO: check to make sure this doesn't get too big.
	frame_counter++;
	
	for (i = 0; i < GAMEKEY_COUNT; i++) {
		// Save the old keys
		old_key[i] = game_key[i];

		// Get new keys
		if ( key[gamekey_to_realkey[i]] )
			game_key[i] = 1;
		else 
			game_key[i] = 0;

		// Output any differences between the old keys and the new keys to a file
		if ( demofile && (old_key[i] != game_key[i]) ) {
				if (!keys_changed) {
						fprintf(demofile, "%lu", frame_counter);
						keys_changed = true;
				}

				// remember, we are writing out GAMEKEYs not REAL keys.
				// e.g. KEY_JUMP, not KEY_SPACEBAR
				fprintf(demofile, " %u %u", i, game_key[i]);
		}
	}
	
	if (keys_changed && demofile) {
			fprintf(demofile, "\n");
	}

	UpdateKeyReleases();
}

bool InputRecord::InitRecorder(CString filename) {
	
	if (demofile) {
		fprintf(stderr, "InputRecord: ERROR already saving demo file.\n");
		return false;
	} 

	demofile = fopen(filename.c_str(), "w");
	
	if (!demofile) {
			fprintf(stderr,	"InputRecord: ERROR can't write to demofile '%s'.\n",
							filename.c_str());
			return false;
	}

	fprintf(stderr, "InputRecord: Recording demo to file '%s'.\n", 
							filename.c_str());

	// write 'DEMO' header + game version number and some extra info
	fprintf(demofile, "DEMO:ninja-engine saved demo file:%s:%s\n",
										VERSION_STRING, ALLEGRO_PLATFORM_STR);
	
	// write the current random seed
	fprintf(demofile, "%i\n", GetGameState()->GetRandomSeed() );
	
	return true;
}

void InputRecord::BeginRecording()	{
				
	if (!demofile) {
		fprintf(stderr,	"InputRecord: ERROR InitRecorder() not called yet!\n");
		return;
	}
	
	ClearKeys();
	ClearKeys(old_key);

	frame_counter = 0;
}

void InputRecord::EndRecording()	{
	fclose(demofile);
	demofile = NULL;
}

InputRecord::InputRecord() : demofile(NULL) {
	old_key.resize(GAMEKEY_COUNT);
}
InputRecord::~InputRecord() {}
