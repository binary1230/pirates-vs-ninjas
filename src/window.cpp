#include "window.h"

#include "gameState.h"
#include "globals.h"
#include "sprite.h"
#include "gameOptions.h"

// DOES NOT USE THE DEPTH BUFFER
// relies on Drawing order (we are 2d after all.)

#define USE_DESKTOP_COLOR_DEPTH

#ifndef USE_DESKTOP_COLOR_DEPTH
#	define DEFAULT_COLOR_DEPTH 16
#else
#	define DEFAULT_COLOR_DEPTH desktop_color_depth()
#endif

// leave this undefined usually.
// #define ALTERNATE_GFX_MODE GFX_XDGA2

DECLARE_SINGLETON(GameWindow)

int screen_size_x = DEFAULT_SCREEN_SIZE_X;
int screen_size_y = DEFAULT_SCREEN_SIZE_Y;

void GameWindow::Screenshot(char* filename) {
	CString file;
	static int screenshot_num = 0;

	if (filename) {
		file = filename;
	} else {
		// Make one up.
		file.Format("ninjas-screenshot%i.png", screenshot_num++);
	}

	fprintf(stderr, " -- saving screenshot '%s'\n", file.c_str());

	save_bitmap(file.c_str(), screen, NULL);
}

void GameWindow::DrawRect(_Rect &r, int col, bool filled) {
	DrawRect(	(int)r.getx1(), (int)r.gety1(), 
						(int)r.getx2(), (int)r.gety2(), 
						col, filled);
}

void GameWindow::DrawRect(	int x1, int y1, 
														int x2, int y2, 
														int col, bool filled) {

	glColor4ub(getr(col), getg(col), getb(col), 255);
	glDisable(GL_TEXTURE_2D);

	if (filled)
		glBegin(GL_QUADS); 
	else
		glBegin(GL_LINES);

	glVertex2f(x1, y1);
	glVertex2f(x2, y1);
	glVertex2f(x2, y2);
	glVertex2f(x1, y2);

	glEnd();

	glEnable(GL_TEXTURE_2D);
	glColor4ub(255, 255, 255, 255);
}

// HACK HACK HACK - get this from somewhere!!
#define FONT_HEIGHT 10

void GameWindow::DrawText(int x, int y, CString text) {
	vector<CString> lines;
	StringSplit(text, OBJECT_TXT_LINE_DELIM, lines);
	int i, max = lines.size();

	int _x = x;
	int _y = y;
	int col = makecol(255,255,255);	// white text color

	for (i = 0; i < max; i++) {
		allegro_gl_printf(main_font, _x, _y, 0.0f, col, lines[i].c_str());
		_y += FONT_HEIGHT;
	}
}

// public function
// NOTE: two things can tell the sprite to flip itself, either
// calling this function with flip_x/y true, or having the sprite
// set its flip_x/y flag to true.  We may have to flip it twice (e.g.
// NOT flip it at all) 
//
// Holy sweetness. Remember that '^' is XOR, and XOR rocks.
void GameWindow::DrawSprite(	Sprite* sprite, int x, int y, 
													bool flip_x, bool flip_y, int alpha) {
	// texture coords
	// we mess with them if flipping
	float tx1 = 0.0f, ty1 = 0.0f;
	float tx2 = 0.0f, ty2 = 1.0f;
	float tx3 = 1.0f, ty3 = 1.0f;
	float tx4 = 1.0f, ty4 = 0.0f;

	int rx = x + sprite->x_offset;

	// clip if needed
	if (rx + sprite->width < 0 || rx > SCREEN_W)
		return;

	// flip x if needed
	if (sprite->flip_x ^ flip_x) {
		tx1 = tx2 = 1.0f;
		tx3 = tx4 = 0.0f;
	}

	int ry = y + sprite->y_offset;

	// clip if needed
	if (ry + sprite->height < 0 || ry > SCREEN_H)
		return;

	// flip y if needed
	if (sprite->flip_y ^ flip_y) {
		ty1 = ty4 = 1.0f;
		ty2 = ty3 = 0.0f;
	}

	glBindTexture(GL_TEXTURE_2D, sprite->texture);

	glBegin(GL_QUADS); 
    glTexCoord2f(tx1, ty1);
  	glVertex2f(rx, ry + sprite->height);

    glTexCoord2f(tx2, ty2);
    glVertex2f(rx, ry);

    glTexCoord2f(tx3, ty3);
    glVertex2f(rx + sprite->width, ry);

    glTexCoord2f(tx4, ty4);
    glVertex2f(rx + sprite->width, ry + sprite->height);
	glEnd();

	// extra params not used yet:
	// sprite->use_alpha, alpha 
}

void GameWindow::SetClearColor(uint r, uint g, uint b) {
	glClearColor(	float(r)/256.0f, 
								float(g)/256.0f, 
								float(b)/256.0f,
								1.0f );
}

int GameWindow::Init( uint _width, uint _height, 
									bool _fullscreen, int _mode) {
	
	int depth = DEFAULT_COLOR_DEPTH;
	int gfx_mode;
	
	width = _width;
	height = _height;

	// Special case: We won't be drawing _anything_
	if (!GAMESTATE->GetGameOptions()->DrawGraphics() ) {
		fprintf(stderr, "GameWindow: DISABLING ALL GRAPHICS\n");
		set_gfx_mode(GFX_TEXT, 320, 240, 0, 0);
		initialized = true;
		return 0;
	}

	install_allegro_gl();
	allegro_gl_clear_settings();
	
	set_color_depth(depth);

	uint gl_flags = AGL_COLOR_DEPTH | AGL_DOUBLEBUFFER | AGL_RENDERMETHOD;

	if (!_fullscreen)
		gl_flags |= AGL_WINDOWED;

	allegro_gl_set(AGL_COLOR_DEPTH, depth);
  allegro_gl_set(AGL_DOUBLEBUFFER, 1);
  // allegro_gl_set(AGL_Z_DEPTH, 24);
  allegro_gl_set(AGL_WINDOWED, !_fullscreen);
  allegro_gl_set(AGL_RENDERMETHOD, 1);
  allegro_gl_set(AGL_SUGGEST, gl_flags);
	
	if (_fullscreen)
      gfx_mode = GFX_OPENGL_FULLSCREEN;
  else
      gfx_mode = GFX_OPENGL;

#	ifdef ALTERNATE_GFX_MODE
	gfx_mode = ALTERNATE_GFX_MODE;
# endif // ALTERNATE_GFX_MODE

	if (get_color_depth() != depth)
		fprintf(stderr, "window: Warning: Asked for %i-bit color mode"
										", got %i-bit instead.\n", depth, get_color_depth());
									
	if (set_gfx_mode(gfx_mode, width, height, 0, 0) != 0) {
		fprintf(stderr, 
						"window: Can't set graphics mode! (%i, %i, fullscreen = %i) \n"
						"Try setting a different graphics mode or try non-fullscreen\n"
						"Allegro error says: '%s'\n"
						"AllegoGL error says: '%s'\n",
						width, height, _fullscreen, allegro_error, allegro_gl_error);
		return -1;
	}
	
	set_window_title(VERSION_STRING);

	// XXX: Font stuff should go in asset manager
	main_font = allegro_gl_convert_allegro_font(
								font, AGL_FONT_TYPE_TEXTURED, -1.0
							);
	assert(main_font != NULL);
	
	if (InitGL())
		initialized = true;
	else
		initialized = false;

	return 0;
}

bool GameWindow::InitGL() {
	glEnable(GL_TEXTURE_2D);
	
	// SET_DEFAULT_GL_ALPHA();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glShadeModel(GL_FLAT);
	glPolygonMode(GL_FRONT, GL_FILL);

	glViewport(0, 0, SCREEN_W, SCREEN_H);
	glMatrixMode(GL_PROJECTION); 
	glLoadIdentity();
	
	glOrtho(0, SCREEN_W, 0, SCREEN_H, -10, 10);
	
	// flip the Y axis
	glScalef(1.0f, -1.0f, 1.0f);

	// move the origin to bottom left
	glTranslatef(0, -SCREEN_H, 0);

  glMatrixMode(GL_MODELVIEW);

	return true;
}

void GameWindow::Clear() {
	glClear(GL_COLOR_BUFFER_BIT);
  glLoadIdentity();
}

void GameWindow::BeginDrawing() {
	glEnable(GL_ALPHA_TEST);
}

void GameWindow::EndDrawing() {
}

// draws the backbuffer to the screen and erases the backbuffer
void GameWindow::Flip() {
	allegro_gl_flip();
}

void GameWindow::Shutdown() {
	if (!initialized)
			return;

	allegro_gl_destroy_font(main_font);

	initialized = false;
}

GameWindow::GameWindow() : initialized(false) {
}

GameWindow::~GameWindow() {}
