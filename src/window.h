// represents a window
#ifndef WINDOW_H
#define WINDOW_H

#include "stdafx.h"
#include "globals.h"
#include "gameBase.h"
#include "rect.h"

class GameState;
class Sprite;

// Modes for creating the display
// Listed in order from fastest, but nastier, to slowest, but nicest

// NO buffering (fastest, looks bad) NOT RECOMMENDED
#define MODE_NOBUFFERING			0

// Double buffering (reduces tearing) recommended default
#define MODE_DOUBLEBUFFERING	1

// Page flipping, (really reduces tearing)
#define MODE_PAGEFLIPPING			2

// Tripe buffering, (slowest, looks nicest) 
// ONLY use this on really fast machines where you can still
// see flickering or tearing.
#define MODE_TRIPLEBUFFERING	3

extern int screen_size_x;
extern int screen_size_y;

//! The onscreen window
class Window : public GameBase {
	protected:
		bool initialized;
		uint width, height;
		int mode;

		//! Points to a surface we can draw to.
		BITMAP* drawing_surface;
		
		// For page flipping
		BITMAP* page[2];
		uint active_page;			// index of the page NOT being
													// displayed (ok to draw on)
		
		// For double buffering
		BITMAP* backbuf;
		
		//! Draw a bitmap, minus the clipping.
		void DrawBitmapAt(	BITMAP* bmp, int x, int y, 
												bool flip_x, bool flip_y, int alpha);

		//! Background clear color
		int clear_color;
		
	public:
		int Init(	GameState* _game_state, 
							uint _width, uint _height, bool _fullscreen = 0,
							int _mode = MODE_DOUBLEBUFFERING);

		void Shutdown();

		inline BITMAP* GetDrawingSurface() {return drawing_surface;};
		void Clear();
		void Flip();

		inline uint Width() {return width;};
		inline uint Height() {return height;};

		void DrawSprite(	Sprite* sprite, int x, int y, 
											bool flip_x=0, bool flip_y=0, int alpha=255);

		void DrawBitmap(	BITMAP* bmp, int x, int y, 
											bool flip_x=0, bool flip_y=0, int alpha=255);

		//! Blit is provided if we need to do something more complex
		void BlitBitmap(  BITMAP* bmp, int source_x, int source_y, 
											int dest_x, int dest_y, int width, int height);

		//! Debug only - draw a bounding rectangle
		void DrawRect(Rect &r, int col);

		inline void SetClearColor(int col) {clear_color = col;};

		//! Take a screenshot, call after Draw()
		//! Leave the filename NULL to use an automatic one
		void Screenshot(char* filename = NULL);

		Window();
		~Window();
};

#endif // WINDOW_H
