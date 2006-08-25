#ifndef INPUT_H
#define INPUT_H

#include "allegro_wrapper.h"
#include <assert.h>
#include "StdString.h"
#include "stl_wrapper.h"

class BaseInput;

#include "gameBase.h"
#include "globals.h"

//! The max number of "player keys" (e.g. JUMP, LEFT, etc)
#define PLAYERKEY_COUNT					5

//! The player keys (these are NOT indices into game_key[])
//! These constants are used with a controller number
//! To get the logical key mapping, shown below this.
#define PLAYERKEY_JUMP 					0
#define PLAYERKEY_LEFT					1
#define PLAYERKEY_RIGHT					2
#define PLAYERKEY_UP					3
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
#define GAMEKEY_START						11
#define GAMEKEY_DEBUGPAUSE			12
#define GAMEKEY_DEBUGSTEP				13
#define GAMEKEY_SCREENSHOT			14

/* End of logical mapping */

//! The max number of defined keys 
#define GAMEKEY_COUNT						15

enum MouseClickType {
	MOUSE_LEFT_BTN,
	MOUSE_RIGHT_BTN,
	MOUSE_MIDDLE_BTN,
	MOUSE_SCROLL_UP,
	MOUSE_SCROLL_DOWN
};

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
//! extremely important, especially for the demos, because it makes 
//! sure that input values do NOT change DURING Update()'s.  
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
		std::vector<int> game_key;

		//! The mapping of real keys to gamekeys (e.g. SPACE to GAMEKEY_JUMP)
		std::vector<int> gamekey_to_realkey;

		//! Whether a key has been released after it was pressed
		std::vector<int> released_key;

		int mouse_x;
		int mouse_y;

		std::vector<bool> mouse_click_status;
		
		//! Load the default key mappings
		void LoadDefaultKeyMappings();
		
		//! Load key mappings from a file
		bool LoadKeyMappings(char* filename);

		//! Clear the default keyboard buffer
		void ClearKeys();

		//! Clear a specific keyboard buffer
		void ClearKeys(std::vector<int> &key_buffer);
		
	public:
		//! Init the input system
		virtual int Init(GameState* _game_state, CString _demo_file) = 0;

		//! Shutdown the input system
		virtual void Shutdown() = 0;

		//! Update the inputs (mice/keys)
		virtual void Update() = 0;

		//! Get the status of a key on a specified controller
		//! note: for player keys (e.g. player 1 JUMP), with this method you 
		//! will need to say Key(PLAYERKEY_JUMP, 1)
		//!
		//! Or, you can skip the controller # and say something like 
		//! Key(GAMEKEY_SCREENSHOT);
		//!
		//! Controller numbers start with 1.
		bool Key(uint gameKey, uint controller_number = 0);

		//! Returns true if a key was first released, then pressed.
		//! Can be used to make sure that a player is pressing and releasing
		//! a key, instead of just holding it down.  
		//!
		//! NOTE: This is just the check. Make sure to call HandleKeyOnce()
		//! as well.
		bool CheckKeyOnce(uint gameKey, uint controller_number = 0);

		//! Call after KeyOnce to make sure that the player must release 
		//! the key before KeyOnce will return true again
		void HandleKeyOnce(uint gameKey, uint controller_number = 0);

		//! Calls CheckKeyOnce() and HandleKeyOnce()
		bool KeyOnce(uint gameKey, uint controller_number = 0);

		int ResolveControllerKey(uint gameKey, uint controller_number);

		void UpdateKeyReleases();

		bool BaseInit();
		
		//! Mouse status
		bool MouseClick(MouseClickType);
		int MouseX();
		int MouseY();
		
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
