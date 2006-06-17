#include <allegro.h>

#include "window.h"
#include "gameState.h"
#include "globals.h"
#include "sprite.h"

int screen_size_x = DEFAULT_SCREEN_SIZE_X;
int screen_size_y = DEFAULT_SCREEN_SIZE_Y;

// public function
void Window::DrawBitmap(BITMAP* bmp, int x, int y, bool flip_x, bool flip_y) {

	// if its position is offscreen, don't draw it.
	if (x + bmp->w < 0 || x >= (int)width || 
			y + bmp->h < 0 || y >= (int)height ) 
			return;

	// Draw the bitmap
	DrawBitmapAt(bmp, x, y, flip_x, flip_y);
}

void Window::DrawRect(Rect &r, int col) {
	rect(drawing_surface, 
									(int)r.getx1(), 
									(int)r.gety1(), 
									(int)r.getx2(), 
									(int)r.gety2(), 
									col);
}

// private: only
void Window::DrawBitmapAt(BITMAP* bmp, int x, int y, bool flip_x, bool flip_y) {
	if (!flip_x) 
		draw_sprite(drawing_surface, bmp, x, y);
	else
		draw_sprite_h_flip(drawing_surface, bmp, x, y);
}

// public function
// NOTE: two things can tell the sprite to flip itself, either
// calling this function with flip_x/y true, or having the sprite
// set its flip_x/y flag to true.  We may have to flip it twice (e.g.
// NOT flip it at all) 
//
// Holy sweetness. Remember that '^' is XOR, and XOR rocks.
void Window::DrawSprite(Sprite* sprite, int x, int y, bool flip_x, bool flip_y) {
	DrawBitmap( sprite->bmp, 
							x + sprite->x_offset, y + sprite->y_offset, 
							sprite->flip_x ^ flip_x, sprite->flip_y ^ flip_y);
}

int Window::Init(	GameState* _game_state, 
									uint _width, uint _height, 
									bool _fullscreen, int _mode) {
	
	int gfx_mode;
	int vheight;
	
	SetGameState(_game_state);

	width = _width;
	height = _height;
	mode = _mode;
	
	if (_fullscreen)
			gfx_mode = GFX_AUTODETECT_FULLSCREEN;
	else
			gfx_mode = GFX_AUTODETECT_WINDOWED;

	if (mode == MODE_PAGEFLIPPING || mode == MODE_TRIPLEBUFFERING)
		vheight = height * 2;
	else 
		vheight = 0;

	int depth = 16;
	set_color_depth(depth);
	/* This code doesn't work with allegro 4.0.x (no get_color_depth())
	 * if (get_color_depth() != depth)
		fprintf(stderr, "window: Warning: Asked for %i-bit color mode, got %i-bit instead.\n", depth, get_color_depth());*/
									
	if (set_gfx_mode(gfx_mode, width, height, 0, vheight) != 0) {
		fprintf(stderr, 
						"window: Can't set graphics mode! (%i, %i, fullscreen = %i) \n"
						"Try setting a different graphics mode or try non-fullscreen\n",
						width, height, _fullscreen);
		return -1;
	}	

	set_window_title(VERSION_STRING);
	
	if (mode == MODE_DOUBLEBUFFERING) {
		clear_bitmap(screen);
		
		// initialize back buffering
		backbuf = create_bitmap(width, height);
		if (!backbuf) {
			fprintf(stderr, "window: can't create back buffer!\n");
			return -2;
		}
		
		clear_bitmap(backbuf);
		drawing_surface = backbuf;
		
	} else if (mode == MODE_PAGEFLIPPING) {
	
		// set up page flipping
		page[0] = create_video_bitmap(width, height);
		page[1] = create_video_bitmap(width, height);

		if ((!page[0]) || (!page[1])) {
			fprintf(stderr, "window: can't setup page flipping!\n");
			return -2;
		}

		active_page = 0;
		drawing_surface = page[active_page];
		
	} else if (mode == MODE_NOBUFFERING) {

		clear_bitmap(screen);
		drawing_surface = screen;
		
	} else {
		
		fprintf(stderr, "window: specified mode not supported!\n");
		return -2;
		
	}

	initialized = true;

	return 0;
}

void Window::Clear() {
	switch (mode) {
		case MODE_PAGEFLIPPING:
			clear_bitmap(page[active_page]);
			break;
		case MODE_DOUBLEBUFFERING:
			clear_bitmap(backbuf);
			break;
		case MODE_NOBUFFERING:
			clear_bitmap(screen);
			break;
		default:
			fprintf(stderr, "ERROR: Unkown buffering mode %u\n", mode);
	}
}

// draws the backbuffer to the screen and erases the backbuffer
void Window::Flip() {
	if (mode == MODE_PAGEFLIPPING) {
		show_video_bitmap(page[active_page]);
	
		if (active_page == 1)
			active_page = 0;
		else 
			active_page = 1;
	
		drawing_surface = page[active_page];
	} else if (mode == MODE_DOUBLEBUFFERING) {
		vsync();
		blit(backbuf, screen, 0, 0, 0, 0, width, height);
	} else if (mode == MODE_NOBUFFERING) {
		// do nothing	
	}
}

void Window::Shutdown() {
	if (!initialized)
			return;

	if (mode == MODE_PAGEFLIPPING) {
		destroy_bitmap(page[0]);
		destroy_bitmap(page[1]);
	} else if (mode == MODE_DOUBLEBUFFERING) {
		destroy_bitmap(backbuf);
	}
	
	drawing_surface = NULL;
	release_screen();
	initialized = false;
}

Window::Window() : initialized(false), drawing_surface(NULL) {
	page[0] = NULL;
	page[1] = NULL;
}
Window::~Window() {}
