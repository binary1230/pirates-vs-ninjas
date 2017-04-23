#ifndef INPUT_H
#define INPUT_H

#include "globals.h"

//! The max number of "player keys" (e.g. JUMP, LEFT, etc)
#define PLAYERKEY_COUNT					7

//! The player keys (these are NOT indices into game_key[])
//! These constants are used with a controller number
//! To get the logical key mapping, shown below this.
#define PLAYERKEY_JUMP 					0
#define PLAYERKEY_LEFT					1
#define PLAYERKEY_RIGHT					2
#define PLAYERKEY_UP					3
#define PLAYERKEY_DOWN					4
#define PLAYERKEY_ACTION1				5
#define PLAYERKEY_ACTION2				6

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// The logical mapping of the keys
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

//! The logical game keys (e.g. JUMP, FIRE, etc)
//! (this must reflect PLAYERKEY_COUNT)
#define PLAYERKEY1_JUMP 				0
#define PLAYERKEY1_LEFT					1
#define PLAYERKEY1_RIGHT				2
#define PLAYERKEY1_UP					3
#define PLAYERKEY1_DOWN					4
#define PLAYERKEY1_ACTION1				6
#define PLAYERKEY1_ACTION2				7

#define PLAYERKEY2_JUMP 				8
#define PLAYERKEY2_LEFT					9
#define PLAYERKEY2_RIGHT				10
#define PLAYERKEY2_UP					11
#define PLAYERKEY2_DOWN					12
#define PLAYERKEY2_ACTION1				13
#define PLAYERKEY2_ACTION2				14

#define GAMEKEY_EXIT					15
#define GAMEKEY_START					16
#define GAMEKEY_DEBUGPAUSE				17
#define GAMEKEY_DEBUGSTEP				18
#define GAMEKEY_SCREENSHOT				19
#define GAMEKEY_TOGGLE_PHYSICS_DISPLAY	20
#define GAMEKEY_SAVE_MAP				21

/* End of logical mapping */

//! The max number of defined keys 
#define GAMEKEY_COUNT						22

enum MouseClickType {
	MOUSE_LEFT_BTN		= 0x00000001,
	MOUSE_RIGHT_BTN		= 0x00000002,
	MOUSE_MIDDLE_BTN	= 0x00000004,

	MOUSE_SCROLL_UP		= 0x00000008,	// these don't really exist in allegro?
	MOUSE_SCROLL_DOWN	= 0x00000010,
};

const int g_iMaxMouseButtonCount = 5;

// The input can come from 3 places:
enum InputType {
	INPUT_RECORDED,			// recorded input, written to a demo file
	INPUT_PLAYBACK,			// played back input, read from a demo file
	INPUT_LIVE					// live input right from the mouse/keyboard
};

// 'Key' type
typedef vector<int> KeyList;

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
class Input {
	
	DECLARE_SINGLETON_CLASS(Input)

	protected:
		// -- COMMON STUFF -- //
		
		// The input type (e.g. playback, record, or live)
		InputType type;
		
		//! The keys currently being pressed (e.g. GAMEKEY_JUMP)
		KeyList game_key;

		//! The mapping of real keys to gamekeys (e.g. SPACE to GAMEKEY_JUMP)
		KeyList gamekey_to_realkey;

		//! Whether a key has been released after it was pressed
		KeyList released_key;

		//! Same thing as above, but for all keys, not just game keys
		//! DO NOT USE FOR IN-GAME CODE, JUST MAP EDITOR/etc.
		char real_released_key[ALLEGRO_KEY_MAX];

		int mouse_x_pos;
		int mouse_y_pos;
		uint mouse_buttons;
		uint released_mouse_buttons;
		
		//! Load the default key mappings
		void LoadDefaultKeyMappings();
		
		//! Load key mappings from a file
		bool LoadKeyMappings(char* filename);

		//! Clear the default keyboard buffer
		void ClearKeys();

		//! Clear a specific keyboard buffer
		void ClearKeys(KeyList &key_buffer);
		
		//! The current demo file being read from, if any
		FILE* demofile;
		
		//! Update key release info
		void UpdateKeyReleases();
		void UpdateRealKeyReleases();
		void UpdateMouseButtonReleases();

		//! Init common stuff for all input types
		bool CommonInit();

		// -- LIVE INPUT STUFF -- //

		//! Init live input stuff
		bool InitLive();

		//! Update live input stuff
		void UpdateLive();

		// -- DEMO PLAYBACK STUFF --	//
		
		//! The frame count we are on [updated by Update()]
		unsigned long frame_counter;

		//! The next frame for which we have data
		unsigned long next_frame_num;

		//! The next frame's data
		KeyList next_frame_data;

		//! True if we are at the end of this file
		bool at_eof;

		//! Reads the next frame from a file
		void GetNextFrameData();

		//! Uses the new frame data as the current data
		void UseNextFrameData();
		
		//! Update playback stuff
		void UpdatePlayback();

		void BeginPlayback();
		void EndPlayback();
		
		// -- DEMO RECORD STUFF -- //
		
		//! The last state of the keys
		KeyList old_key;
		
		//! Initialize demo recording stuff
		bool InitRecorder(std::string filename);
		
		//! Update demofile recorder
		void UpdateRecord();
		
		void BeginRecording();
		void EndRecording();

		//! Do joystick updates
		//! Implemented as a quick hack for now
		void DoJoystickUpdateHack();
		
	public:
		//! Init the input system
		int Init();

		//! Init demo playback stuff
		bool InitPlayback(std::string filename, bool seed_engine = true);

		//! Start using this input
		void Begin();

		//! End using this input
		void End();

		//! Get the input type
		inline InputType GetInputType() {return type;};

		//! Shutdown the input system
		void Shutdown();

		//! Update the inputs (mice/keys)
		void Update();

		//! Get the status of a key on a specified controller
		//! note: for player keys (e.g. player 1 JUMP), with this method you 
		//! will need to say Key(PLAYERKEY_JUMP, 1)
		//!
		//! Or, you can skip the controller # and say something like 
		//! Key(GAMEKEY_SCREENSHOT);
		//!
		//! Controller numbers start with 1.
		bool Key(uint gameKey, uint controller_number = 0);

		//! Same as Key(), but Sets the status
		void SetKey(uint gameKey, uint controller_number = 0, bool value = 1);

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

		//! Calls CheckKeyOnce() and HandleKeyOnce() in that order
		bool KeyOnce(uint gameKey, uint controller_number = 0);

		int ResolveControllerKey(uint gameKey, uint controller_number);
	
		//! Mouse status
		bool MouseButton(MouseClickType);
		int MouseX();
		int MouseY();
			
		virtual ~Input();

		//! DO NOT USE THESE FOR IN-GAME CODE, JUST MAP EDITOR/etc.
		bool CheckRealKeyOnce(uint iKeyNum) const;
		void HandleRealKeyOnce(uint iKeyNum);
		bool RealKeyOnce(uint iKeyNum);
		
		bool MouseButtonOnce(MouseClickType t);
		void HandleMouseButtonOnce(MouseClickType t);
		bool CheckMouseButtonOnce(MouseClickType t) const;
};

#define INPUT Input::GetInstance()

#endif // INPUT_H
