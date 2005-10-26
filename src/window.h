// represents a window
#ifndef WINDOW_H
#define WINDOW_H

#include <allegro.h>
#include <stdio.h>

// class Window;

#include "globals.h"
#include "gameState.h"
#include "gameBase.h"

class Window : public GameBase {
	protected:
		bool initialized;

		uint width, height;

		// For page flipping
		BITMAP* page[2];
		uint drawing_page;		// index of the page NOT being
													// displayed (ok to draw on)
		
	public:
		int Init(	GameState* _game_state, 
							uint width, uint height, bool fullscreen = 0);

		void Shutdown();

		inline BITMAP* GetDrawingSurface() {return page[drawing_page];};
		void Flip();

		inline uint Width() {return width;};
		inline uint Height() {return height;};

		Window();
		~Window();
};

#endif // WINDOW_H
