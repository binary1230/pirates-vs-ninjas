#include "stdafx.h"

#ifndef AI_INPUT_EVENT_H
#define AI_INPUT_EVENT_H

#include "globals.h"

// Represents an Input Event from a demo file
// e.g. (at time (frame#) 36, the user is now NOT pressing button 12)

struct InputEvent {
	long time;						// the time (in frames) when this was pressed
	unsigned int button;	// which button we are referring to
	bool state;						// what is it's state (on or off)
};

#endif // AI_INPUT_EVENT_H
