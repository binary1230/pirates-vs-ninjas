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

	if (set_gfx_mode(gfx_mode, width, height, 0, height*2) != 0) {
		fprintf(stderr, 
						"window: Can't set graphics mode! (%i, %i, fullscreen = %i) \n"
						"Try setting a different graphics mode or try non-fullscreen\n",
						width, height, _fullscreen);
		return -1;
	}	

	set_window_title(VERSION_STRING);
	
	// clear_bitmap(screen);

	// initialize back buffering
	/*backbuf = create_bitmap(width, height);
	if (!backbuf) {
		fprintf(stderr, "window: can't create back buffer!\n");
		return -1;
	}
	
	clear_bitmap(backbuf);*/

	// set up page flipping
	page[0] = create_video_bitmap(width, height);
	page[1] = create_video_bitmap(width, height);

	if ((!page[0]) || (!page[1])) {
		fprintf(stderr, "window: can't setup page flipping!\n");
		return -1;
	}

	drawing_page = 0;
	initialized = true;

	return 0;
}

// draws the backbuffer to the screen and erases the backbuffer
void Window::Flip() {
	/*vsync();
	blit(backbuf, screen, 0, 0, 0, 0, width, height);
	clear_bitmap(backbuf);*/
	show_video_bitmap(page[drawing_page]);
	
	if (drawing_page == 1)
		drawing_page = 0;
	else 
		drawing_page = 1;
	
	clear_bitmap(page[drawing_page]);
}

void Window::Shutdown() {
	if (!initialized)
			return;

	destroy_bitmap(page[0]);
	destroy_bitmap(page[1]);
		 	
	// destroy_bitmap(backbuf);
	release_screen();
	initialized = false;
}

Window::Window() : initialized(false) {
	page[0] = NULL;
	page[1] = NULL;
}
Window::~Window() {}
