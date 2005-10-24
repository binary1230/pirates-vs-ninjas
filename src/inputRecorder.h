#ifndef RECORDER_INPUT_H
#define RECORDER_INPUT_H

class InputRecorder;

#include "globals.h"
#include "input.h"

//! Input Recording class

//! Records the state of the input at every frame
//! and outputs it to a file for playback later
class InputRecorder : public BaseInput {
	int Init();
	void Shutdown();
				
	bool Key(unsigned char index);
	int MouseX();
	int MouseY();
};

#define // RECORDER_INPUT_H
