// represents a window
#ifndef WINDOW_H
#define WINDOW_H

#include <allegro.h>
#include <stdio.h>

class Window;

#include "globals.h"
#include "gameState.h"
#include "gameBase.h"

class Window : public GameBase {
	protected:
		bool initialized;

		uint width, height;
		BITMAP* backbuf;
		
	public:
		int Init(	GameState* _game_state, 
							uint width, uint height, bool fullscreen = 0);

		void Shutdown();

		inline BITMAP* GetBackBuffer() {return backbuf;};
		void Flip();

		Window();
		~Window();
};

#endif // WINDOW_H
