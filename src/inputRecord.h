#ifndef INPUT_RECORD_HANDLER_H
#define INPUT_RECORD_HANDLER_H

#include "allegro_wrapper.h"
#include <stdio.h>
#include "stl_wrapper.h"

class InputRecord;

#include "input.h"

//! Reads live input and records it to a file (used for recording demos)
class InputRecord : public BaseInput {
	
	protected:
		//! The current demo file being written
		FILE* demofile;
		
		//! The last state of the keys
		std::vector<int> old_key;
		
		//! The frame count we are on [updated by Update()]
		unsigned long frame_counter;

		bool InitRecorder(CString filename);
	
	public:
		int Init(GameState* _game_state, CString _demo_file);
		void Shutdown();

		void Update();
		
		void BeginRecording();
		void EndRecording();

		void BeginPlayback()	{NotSupported();}
		void EndPlayback()	{NotSupported();}

		InputRecord();
		~InputRecord();
};

#endif // InputRecordHANDLER_H
