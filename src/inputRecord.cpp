#include "inputRecord.h"

int InputRecord::Init(GameState* _game_state, char* _demo_file) {
	SetGameState(_game_state);
				
	install_mouse();
	install_keyboard();
	
	LoadDefaultKeyMappings();

	if (_demo_file)
		return InitRecorder(_demo_file);
	else 
		return 0;
}

void InputRecord::Shutdown() {
	remove_mouse();
	remove_keyboard();

	if (demofile) {
		fprintf(stderr, "WARN: closing demofile (currently in progress)");
		fclose(demofile);
	}
}

bool InputRecord::Key(uint gameKey) {
	return game_key[gameKey];
}

//! Update the state of the input

//! This is A little complicated..
//! First we save the old keys being pressed.
//! Then we get the new keys being pressed.
//! If anything is different, write it to the demo with the frame counter
void InputRecord::Update() {
	
	uint i;
	bool keys_changed = false;
	
	frame_counter++;
	
	for (i = 0; i < GAMEKEY_COUNT; i++) {
		// Save the old keys
		old_key[i] = game_key[i];

		// Get new keys
		game_key[i] = key[gamekey_to_realkey[i]];

		// Output any differences between the old and new to a file
		if ( demofile && (old_key[i] != game_key[i]) ) {
				if (!keys_changed) {
						fprintf(demofile, "%u", frame_counter);
						keys_changed = true;
				}

				fprintf(demofile, " %u %u", i, game_key[i]);
		}
	}
	
	if (keys_changed && demofile) {
			fprintf(demofile, "\n");
	}
}

bool InputRecord::InitRecorder(char* filename) {
	
	if (demofile) {
		fprintf(stderr, "InputRecord: ERROR already saving demo file.\n");
		return false;
	} 

	demofile = fopen(filename, "w");
	
	if (!demofile) {
			fprintf(stderr,	"InputRecord: ERROR can't write to demofile '%s'.\n",
							filename);
			return false;
	}

	ClearKeys();
	ClearKeys(old_key);

	return true;
}

void InputRecord::BeginRecording()	{
				
	ClearKeys();
	ClearKeys(old_key);

	if (!demofile) {
		fprintf(stderr,	"InputRecord: ERROR InitRecorder() not called yet!\n");
		return;
	}
	
	frame_counter = 0;

	// write 4 byte 'DEMO' to file
	fwrite("DEMO", 4, 1, demofile);

	// write the random seed
	fprintf(demofile, "%i\n", GetGameState()->GetRandomSeed() );
}

void InputRecord::EndRecording()	{
	fclose(demofile);
}

InputRecord::InputRecord() : demofile(NULL) {}
InputRecord::~InputRecord() {}
