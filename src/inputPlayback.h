/* Live Input 
 *
 * Repsonsible for handling 'live' input (keyboard, mouse, etc)
 */

#ifndef InputPlaybackHANDLER_H
#define InputPlaybackHANDLER_H

#include <allegro.h>
#include <stdio.h>

class InputPlayback;

#include "globals.h"
#include "input.h"
#include "gameState.h"

//! Playback recorded input

//! Reads input devices in realtime (used for normal game play)
class InputPlayback : public BaseInput {
	
	protected:
		//! The current demo file being read from
		FILE* demofile;
		
		//! The frame count we are on [updated by Update()]
		unsigned long frame_counter;

		bool InitPlayback(char* filename);

		//! The next frame for which we have data
		unsigned long next_frame_num;
		int next_frame_data[GAMEKEY_COUNT];

		//! True if we are at the end of this file
		bool at_eof;

		//! Reads the next frame from a file
		void GetNextFrameData();

		//! Uses the new frame data as the current data
		void UseNextFrameData();

	public:
		int Init(GameState* _game_state, char* _demo_file = NULL);
		void Shutdown();

		void Update();
		bool Key(uint gameKey);
		
		void BeginPlayback();
		void EndPlayback();

		void BeginRecording() {NotSupported();}
		void EndRecording() {NotSupported();}

		InputPlayback();
		~InputPlayback();
};

#endif // InputPlaybackHANDLER_H
