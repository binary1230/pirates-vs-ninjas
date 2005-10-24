#include "window.h"

int Window::Init(	GameState* _game_state, 
									uint _width, uint _height, bool _fullscreen) {
	
	int gfx_mode;

	SetGameState(_game_state);

	width = _width;
	height = _height;
	
	if (_fullscreen)
			gfx_mode = GFX_AUTODETECT_FULLSCREEN;
	else
			gfx_mode = GFX_AUTODETECT;

	if (set_gfx_mode(gfx_mode, width, height, 0, 0) != 0) {
		fprintf(stderr, 
						"window: Can't set graphics mode! (%i, %i, fullscreen = %i) \n"
						"Try setting a different graphics mode or try non-fullscreen\n",
						width, height, _fullscreen);
		return -1;
	}	

	set_window_title(VERSION_STRING);
	
	clear_bitmap(screen);

	// initialize back buffering
	backbuf = create_bitmap(width, height);
	if (!backbuf) {
		fprintf(stderr, "window: can't create back buffer!\n");
		return -1;
	}
	
	clear_bitmap(backbuf);

	initialized = true;

	return 0;
}

// draws the backbuffer to the screen and erases the backbuffer
void Window::Flip() {
	blit(backbuf, screen, 0, 0, 0, 0, width, height);
	clear_bitmap(backbuf);
}

void Window::Shutdown() {
	if (!initialized)
			return;

	destroy_bitmap(backbuf);
	release_screen();
	initialized = false;
}

Window::Window() : initialized(false), backbuf(NULL) {}
Window::~Window() {}
