/* Live Input 
 *
 * Repsonsible for handling 'live' input (keyboard, mouse, etc)
 */

#ifndef INPUTLIVEHANDLER_H
#define INPUTLIVEHANDLER_H

#include <allegro.h>
#include <stdio.h>

class InputLive;

#include "globals.h"
#include "input.h"

//! Live input 

//! Reads input devices in realtime (used for normal game play)
class InputLive : public BaseInput {
	
	protected:
		//! The keys currently being pressed (e.g. GAMEKEY_JUMP)
		int game_key[GAMEKEY_COUNT];

		//! They mapping of real keys to gamekeys (e.g. SPACE to GAMEKEY_JUMP)
		int gamekey_to_realkey[GAMEKEY_COUNT];
		
		//! Load the default key mappings
		void LoadDefaultKeyMappings();
		
		//! Load key mappings from a file
		bool LoadKeyMappings(char* filename);

		//! Zero out the keyboard buffer
		void ClearKeys();
		
		//! Display 'not supported'
		inline bool NotSupported() {
			fprintf(stderr, "Not supported.\n");
			return false;
		}
	
	public:
		int Init(GameState* _game_state);
		void Shutdown();

		void Update();
				
		bool Key(enum GameKeys gameKey);

		bool InitRecorder(char* filename) {return NotSupported();}
		void BeginRecording()	{NotSupported();}
		void EndRecording()	{NotSupported();}
		void BeginPlayback()	{NotSupported();}
		void EndPlayback()	{NotSupported();}
		void SaveRandomSeed()	{NotSupported();}
		void GetRandomSeed()	{NotSupported();}

		InputLive();
		~InputLive();
};

#endif // INPUTLIVEHANDLER_H
