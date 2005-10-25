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
#define GAMEKEY_JUMP 		0
#define GAMEKEY_LEFT		1	
#define GAMEKEY_RIGHT		2
#define GAMEKEY_UP			3
#define GAMEKEY_DOWN		4
#define GAMEKEY_EXIT		5

#define GAMEKEY_COUNT 	6

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
	protected:
		//! The keys currently being pressed (e.g. GAMEKEY_JUMP)
		int game_key[GAMEKEY_COUNT];

		//! The mapping of real keys to gamekeys (e.g. SPACE to GAMEKEY_JUMP)
		int gamekey_to_realkey[GAMEKEY_COUNT];
		
		//! Load the default key mappings
		void LoadDefaultKeyMappings();
		
		//! Load key mappings from a file
		bool LoadKeyMappings(char* filename);

		//! Zero out the keyboard buffer
		void ClearKeys(int *key_buffer = NULL);
		
	public:
		//! Init the input system
		virtual int Init(GameState* _game_state) = 0;

		//! Shutdown the input system
		virtual void Shutdown() = 0;

		//! Update the internal cache of inputs (mice/keys)
		virtual void Update() = 0;
		
		//! Get the status of a key (use 'enum GameKeys')
		virtual bool Key(uint gameKey) = 0;

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

		BaseInput();
		virtual ~BaseInput() = 0;
};

#endif // INPUT_H
