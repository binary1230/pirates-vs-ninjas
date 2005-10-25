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
	
	public:
		int Init(GameState* _game_state);
		void Shutdown();

		void Update();
				
		bool Key(uint gameKey);

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
