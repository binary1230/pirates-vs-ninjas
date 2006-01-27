#include "inputPlayback.h"
#include "globals.h"
#include "gameState.h"

int InputPlayback::Init(GameState* _game_state, CString _demo_file) {
	SetGameState(_game_state);
				
	LoadDefaultKeyMappings();
	
	install_mouse();
	install_keyboard();

	if( !InitPlayback(_demo_file) )
		return -1;
	
	return 0;
}

bool InputPlayback::InitPlayback(CString filename) {

	const uint BUF_SIZE = 256; 
	char line2[256];
	bool error = false;

	at_eof = false;
	uint seed;
				
	if (demofile) {
		fprintf(stderr, "InputPlayback: ERROR already reading demo file.\n");
		return false;
	} 

	demofile = fopen(filename.c_str(), "r");
	
	if (!demofile) {
			fprintf(stderr,	"InputPlayback: ERROR can't open demofile '%s'.\n",
							filename.c_str());
			return false;
	}
	
	fprintf(stderr, "InputRecord: Playing back demo from file '%s'.\n", 
							filename.c_str());

	// 1st line2: 'DEMO' header + version info
	// (todo.. we could check for engine version numbers in this line2)
	if ( error	|| !fgets(line2, BUF_SIZE, demofile) 
							|| strncmp(line2, "DEMO", 4) != 0 )
		error = true;

	// 2nd line2, random seed
	if ( error	|| !fgets(line2, BUF_SIZE, demofile)
							|| sscanf(line2, "%u\n", &seed) != 1) {
		error = true;
	} else {
		GetGameState()->SetRandomSeed(seed);
		fprintf(stderr, "InputPlayback: Using random seed %u\n", seed);
	}

	if (error) {
		fclose(demofile);
		demofile = NULL;
		at_eof = true;
	}

	frame_counter = 0;
	
	ClearKeys();
	ClearKeys(next_frame_data);
	GetNextFrameData();

	return !error;
}

// pass key_buffer = NULL in order to use the default key buffer
// (which is usually what you want)
void InputPlayback::UseNextFrameData() {	
	uint i;
	for (i = 0; i < GAMEKEY_COUNT; i++) {
		game_key[i] = next_frame_data[i];
	}
}

//! Reads the next frame from the demo file.  Closes the
//! file if we are at the end of file.
void InputPlayback::GetNextFrameData() {
	const uint BUF_SIZE = 512;
	char buf[BUF_SIZE];
	char *s = buf;
	bool error = false;	

	uint next_frame;
	uint key, value;
	uint count = 0;

	if (!demofile || at_eof)
		return;
	
	if (!fgets(buf, BUF_SIZE, demofile)) {
		at_eof = true;
		return;
	}
	
	// the file format is like this:
	// "frame_number key1 value1 [key2] [value2]\n"
	
	// read the frame number
	count = sscanf(buf, "%u", &next_frame);
	if (count != 1 || next_frame <= frame_counter)
		error = true;
		
	// go to next space
	if (error || (s = strchr(buf, ' ')) == NULL)
		error = true;
	else
		next_frame_num = next_frame;
			
	// read each key,value pair,
	// overwrite existing values in next_data_
	count = 2;
	while (!error && count == 2 && s) {
						
		s++;	// skip over current space
		count = sscanf(s, "%u %u", &key, &value);
			
		if (count == 2) {
			next_frame_data[key] = value;

			// skip 2 spaces
			s = strchr(s, ' ');
			if (s) 
				s = strchr(s+1, ' ');	// will be NULL at end of the line
			else
				count = 1;						// fall through to error handler
		}
			
		if (count == 1) {
			// total badness.. we need to handle this better,
			// but just DIE right here.
			fprintf(stderr, "HUGE HUGE INTERNAL ERROR: Could only read one value\n"
											"from the demo file, it is probably corrupted.\n");
			error = true;
			exit(-1);
		}
			
	}	
}

//! Update the state of the input
void InputPlayback::Update() {
	
	// TODO: check to make sure this doesn't get too big.
	frame_counter++;

	// Read keystrokes from Demo file if they exist
	if (frame_counter == next_frame_num && !at_eof) {
			UseNextFrameData();	
			GetNextFrameData();
	}	

	// SPECIAL EXCEPTION
	// everything comes back from the demo file,
	// but we still allow the user to press GAMEKEY_EXIT key LIVE
	// so they can exit the demo manually
	if (key[gamekey_to_realkey[GAMEKEY_EXIT]])
		game_key[GAMEKEY_EXIT] = 1;	
}

void InputPlayback::BeginPlayback()	{
				
	ClearKeys();

	if (!demofile) {
		fprintf(stderr,	"InputPlayback: ERROR InitPlayback() not called yet!\n");
		return;
	}	

	frame_counter = 0;
}

void InputPlayback::EndPlayback()	{
	fclose(demofile);
	demofile = NULL;
}

void InputPlayback::Shutdown() {
	if (demofile) {
		fprintf(stderr, "WARN: closing demofile (currently in progress)");
		fclose(demofile);
		demofile = NULL;
	}
	remove_mouse();
	remove_keyboard();
}

InputPlayback::InputPlayback() : demofile(NULL) {
	next_frame_data.resize(GAMEKEY_COUNT);
}
InputPlayback::~InputPlayback() {}
