#ifndef INPUT_H
#define INPUT_H

#include <allegro.h>
#include <vector>
#include <assert.h>
#include <StdString.h>
using namespace std;

class BaseInput;

#include "gameBase.h"
#include "globals.h"

//! The max number of "player keys" (e.g. JUMP, LEFT, etc)
#define PLAYERKEY_COUNT					5

//! The player keys (these are NOT indices into game_key[])
#define PLAYERKEY_JUMP 					0
#define PLAYERKEY_LEFT					1
#define PLAYERKEY_RIGHT					2
#define PLAYERKEY_UP						3
#define PLAYERKEY_DOWN					4

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// The logical mapping of the keys
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

//! The logical game keys (e.g. JUMP, FIRE, etc)
//! (this must reflect PLAYERKEY_COUNT)
#define PLAYERKEY1_JUMP 				0
#define PLAYERKEY1_LEFT					1
#define PLAYERKEY1_RIGHT				2
#define PLAYERKEY1_UP						3
#define PLAYERKEY1_DOWN					4

#define PLAYERKEY2_JUMP 				5
#define PLAYERKEY2_LEFT					6
#define PLAYERKEY2_RIGHT				7
#define PLAYERKEY2_UP						8
#define PLAYERKEY2_DOWN					9

#define GAMEKEY_EXIT						10

/* End of logical mapping */

//! The max number of defined keys 
#define GAMEKEY_COUNT						11

//! Input Base Class
 
//! The only reason for having a base class is
//! to make derived classes - one which can do 
//! real input live from keyboard, mouse, or joystick
//! and the other class to be able to record the input
//! and play it back for the sake of a demo game.
//!
//! The underlying game should not notice the difference
//! between live and recorded stuff.

//! Common input subsystem

//! The base input class is used for 3 classes:
//! The LiveInput class, the RecordInput class, and
//! PlaybackInput class.
//! -
//! Every frame, Update() MUST be called.  This freezes the
//! current state of the buttons/mouse for this frame.  This is
//! extremely important, especially for the demos, to make sure 
//! input values do NOT change DURING Update()'s.  
//! -
//! If the derived class is LiveInput then input received live
//! from the input devices will be used
//! -
//! If the derived class is RecordInput, it will save the state of
//! the input devices at EVERY frame on Update() to a file.
//! -
//! If the derived class is PlaybackInput, it will load the state
//! of the keys from a file each frame on Update().
//! -
//! The class also saves random seed during recording to make playback
//! be deterministic
class BaseInput : public GameBase {
	protected:
		//! The keys currently being pressed (e.g. GAMEKEY_JUMP)
		vector<int> game_key;

		//! The mapping of real keys to gamekeys (e.g. SPACE to GAMEKEY_JUMP)
		vector<int> gamekey_to_realkey;
		
		//! Load the default key mappings
		void LoadDefaultKeyMappings();
		
		//! Load key mappings from a file
		bool LoadKeyMappings(char* filename);

		//! Clear the default keyboard buffer
		void ClearKeys();

		//! Clear a specific keyboard buffer
		void ClearKeys(vector<int> &key_buffer);
		
	public:
		//! Init the input system
		virtual int Init(GameState* _game_state, CString _demo_file) = 0;

		//! Shutdown the input system
		virtual void Shutdown() = 0;

		//! Update the inputs (mice/keys)
		virtual void Update() = 0;
		
		//! Get the status of a key 
		//! note: for player keys (e.g. player 1 JUMP), with this method 
		//! you will need to say Key(PLAYERKEY1_JUMP).  
		//!
		//! Controller numbers start at 1.
		//!
		//! [See the other Key() method too.]
		bool Key(uint gameKey);
		
		//! Get the status of a key on a specified controller
		//! note: for player keys (e.g. player 1 JUMP), with this method you 
		//! will need to say Key(PLAYERKEY_JUMP, 1)
		//
		//! Controller numbers start with 1.
		bool Key(uint gameKey, uint controller_number);

		//! Get the mouse X coordinate
		// virtual int MouseX() = 0;

		//! Get the mouse Y coordinates
		// virtual int MouseY() = 0;
		
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
