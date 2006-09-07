#ifndef INPUT_PLAYBACK_kHANDLER_H
#define INPUT_PLAYBACK_kHANDLER_H

#include "stdafx.h"
#include "input.h"

class GameState;

//! Play back recorded input from a file (used for playing back demos)
class InputPlayback : public BaseInput {
	
	protected:
		//! The current demo file being read from
		FILE* demofile;
		
		//! The frame count we are on [updated by Update()]
		unsigned long frame_counter;

		bool InitPlayback(CString filename);

		//! The next frame for which we have data
		unsigned long next_frame_num;
		vector<int> next_frame_data;

		//! True if we are at the end of this file
		bool at_eof;

		//! Reads the next frame from a file
		void GetNextFrameData();

		//! Uses the new frame data as the current data
		void UseNextFrameData();

	public:
		int Init(GameState* _game_state, CString _demo_file);
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
