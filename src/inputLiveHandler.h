#ifndef INPUTLIVEHANDLER_H
#define INPUTLIVEHANDLER_H

#include "stdafx.h"
#include "input.h"

class InputLive;

//! Reads input devices in realtime (used for normal game play, not demos)
class InputLive : public BaseInput {
	
	protected:
	
	public:
		int Init(GameState* _game_state, CString _demo_file);
		void Shutdown();

		void Update();
				
		bool Key(uint gameKey);

		void MouseX();
		void MouseY();
		void MouseClick(MouseClickType);

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
