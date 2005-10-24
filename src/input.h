/* Input Base Class
 *
 * The only reason for having a base class is
 * to make derived classes - one which can do 
 * real input live from keyboard, mouse, or joystick
 * and the other class to be able to record the input
 * and play it back for the sake of a demo game.
 *
 * The underlying game should not notice the difference
 * between live and recorded stuff.
 */

#ifndef INPUT_H
#define INPUT_H

#include <allegro.h>

class BaseInput;

#include "globals.h"
#include "gameBase.h"

//! The logical game keys (e.g. JUMP, FIRE, etc)
enum GameKeys {
	GAMEKEY_JUMP,
	GAMEKEY_LEFT, GAMEKEY_RIGHT, GAMEKEY_UP, GAMEKEY_DOWN,
	GAMEKEY_EXIT
};

//! The count of game keys
#define GAMEKEY_COUNT	(sizeof(enum GameKeys) - 1)

//! Base input class

//! The base input class is used for 3 classes:
//! The LiveInput class, the RecordInput class, and
//! PlaybackInput class.
//! -
//! Every frame, Update() MUST be called.  This freezes the
//! current state of the buttons/mouse for this frame.  This is
//! extremely important, especially for the demos, to make sure 
//! input values do NOT change between frames.  
//! -
//! If the derived class is LiveInput then the input Live from the 
//! input devices will be show
//! -
//! If the derived class is RecordInput, it will save the state of
//! the input devices at EVERY frame on Update() to a file.
//! -
//! If the derived class is PlaybackInput, it will load the state
//! of the keys from a file each frame on Update().
//! -
//! Don't forget to save the random seed in this class, otherwise demos
//! will not play back correctly.
class BaseInput : public GameBase {
	public:
		//! Init the input system
		virtual int Init(GameState* _game_state) = 0;

		//! Shutdown the input system
		virtual void Shutdown() = 0;

		//! Update the internal cache of inputs (mice/keys)
		virtual void Update() = 0;
		
		//! Get the status of a key (use 'enum GameKeys')
		virtual bool Key(enum GameKeys gameKey) = 0;

		//! Get the mouse X coordinate
		// virtual int MouseX() = 0;

		//! Get the mouse Y coordinates
		// virtual int MouseY() = 0;
		
		//! RECORD: Initialize recording engine
		virtual bool InitRecorder(char* filename) = 0;
		
		//! RECORD: Begin recording input (for demos)
		virtual void BeginRecording() = 0;

		//! RECORD: End Recording input (for demos)
		virtual void EndRecording() = 0;
		
		//! PLAYBACK: Begin playback of input (for demos)
		virtual void BeginPlayback() = 0;

		//! PLAYBACK: End playback of input (for demos)
		virtual void EndPlayback() = 0;

		//! Save the random seed used in this game (for demos)
		virtual void SaveRandomSeed() = 0;

		//! Get the random seed used in this game (for demos)
		virtual void GetRandomSeed() = 0;
	
		BaseInput();
		virtual ~BaseInput() = 0;
};

#endif // INPUT_H
