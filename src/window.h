// represents a window
#ifndef WINDOW_H
#define WINDOW_H

#include <allegro.h>
#include <stdio.h>

#include "globals.h"

class Window {
	protected:
		bool initialized;

		uint width, height;
		BITMAP* backbuf;
		
	public:
		int Init(uint width, uint height, bool fullscreen = 0);
		void Shutdown();

		BITMAP* GetBackBuffer();
		void Flip();

		Window();
		~Window();
};

#endif // WINDOW_H
