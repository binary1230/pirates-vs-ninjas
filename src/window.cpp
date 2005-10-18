#include "window.h"

Window::Window() : backbuf(NULL) {}
Window::~Window() {}

int Window::Init(uint _width, uint _height, bool _fullscreen) {

	int gfx_mode;

	width = _width;
	height = _height;
	
	// initialize all allegro stuff
	allegro_init();

	if (_fullscreen)
			gfx_mode = GFX_AUTODETECT_FULLSCREEN;
	else
			gfx_mode = GFX_AUTODETECT;

	if (set_gfx_mode(gfx_mode, width, height, 0, 0) != 0) {
		fprintf(stderr, "window: Can't set graphics mode! (%i, %i) \n", width, height);
		return -1;
	}	
	
	clear_bitmap(screen);

	// initialize back buffering
	backbuf = create_bitmap(width, height);
	if (!backbuf) {
		fprintf(stderr, "window: can't create back buffer!\n");
		return -1;
	}
	
	clear_bitmap(backbuf);

	install_mouse();

	return 0;
}

// draws the backbuffer to the screen and erases the backbuffer
void Window::Flip() {
	blit(backbuf, screen, 0, 0, 0, 0, width, height);
	clear_bitmap(backbuf);
}

void Window::Shutdown() {
	destroy_bitmap(backbuf);
	release_screen();
	allegro_exit();
}

BITMAP* Window::GetBackBuffer() {
	return backbuf;
}
