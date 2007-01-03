// represents a window
#ifndef WINDOW_H
#define WINDOW_H

#include "stdafx.h"
#include "globals.h"
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
class GameWindow {

	DECLARE_SINGLETON_CLASS(GameWindow)

	protected:
		bool initialized;
		uint width, height;
		FONT* main_font;
			
		//! Init openGL stuff
		bool InitGL();
		
	public:
		int Init(	uint _width, uint _height, bool _fullscreen = 0,
							int _mode = MODE_DOUBLEBUFFERING);

		void Shutdown();

		void Clear();
		void Flip();

		inline uint Width() {return width;};
		inline uint Height() {return height;};

		void DrawSprite(	Sprite* sprite, int x, int y, 
											bool flip_x=0, bool flip_y=0, 
											bool use_rotation=false, float rotate_angle=0.0f,
											GLuint alpha=255);
	
		//! Draw text at specified XY
		void DrawText(int x, int y, CString text);

		//! Draw a solid rectange
		void DrawRect(_Rect &r, int col,bool filled=false);
		void DrawRect(int x1, int y1, int x2, int y2, int color, bool filled=false);

		void SetClearColor(uint r, uint g, uint b);

		void BeginDrawing();
		void EndDrawing();

		//! Take a screenshot, call directly after Draw()
		//! Leave the filename NULL to guess an 
		//! automatically incrementing filename
		void Screenshot(char* filename = NULL);

		virtual ~GameWindow();

		friend class MapSaver;
};

#define WINDOW GameWindow::GetInstance()

#endif // WINDOW_H
