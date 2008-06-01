#include "stdafx.h"
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

	TRACE(" -- saving screenshot '%s'\n", file.c_str());

	save_bitmap(file.c_str(), screen, NULL);
}
	
void GameWindow::DrawFade() {
	if (fading_state == FADED_NONE)
		return;

	DrawRect(0,0,width,height,0,true,fade_alpha);
}

void GameWindow::Draw() {
	DrawFade();
}

void GameWindow::Update() {
	UpdateFade();
}

void GameWindow::UpdateFade() {

	if (fading_state == FADED_NONE || fading_state == FADED_OUT)
		return;

	if (fading_state == FADING_IN) {
		fade_alpha -= fade_rate;
		if (fade_alpha <= 0)
			fading_state = FADED_NONE;
	} else if (fading_state == FADING_OUT) {
		fade_alpha += fade_rate;
		if (fade_alpha >= 255)
			fading_state = FADED_OUT;
	}
}

void GameWindow::SetFadedIn() {
	fade_alpha = 0;
	fading_state = FADED_NONE;
}

void GameWindow::SetFadedOut() {
	fade_alpha = 255;
	fading_state = FADED_OUT;
}

void GameWindow::FadeOut(int rate /*=1*/) {
	fade_rate = rate;
	fade_alpha = 0;
	fading_state = FADING_OUT;
}

void GameWindow::FadeIn(int rate /*=1*/) {
	fade_rate = rate;
	fade_alpha = 255;
	fading_state = FADING_IN;
}

void GameWindow::DrawRect(_Rect &r, int col, bool filled, int alpha) {
	DrawRect(	(int)r.getx1(), (int)r.gety1(), 
						(int)r.getx2(), (int)r.gety2(), 
						col, filled, alpha);
}

// Draws a level-sized gradient
// The idea is that the gradient is the height of the LEVEL not the SCREEN
// So we have to figure out given the level height, screen height, and current
// position of the camera, what two colors we need to draw on a quad in order
// to make it look like there is one continuous gradient going up the level

// Usually the screen height is smaller than the level height
void GameWindow::DrawBackgroundGradient(	int bottom_col, int top_col, 
																					int bottom_y, int top_y, 
																					int level_height) {
	
	// get the color differences for computing the new colors
	int col_diff_r = getr(top_col) - getr(bottom_col);
	int col_diff_g = getg(top_col) - getg(bottom_col);
	int col_diff_b = getb(top_col) - getb(bottom_col);

	// sanity check.
	if (top_y > level_height)
		top_y = level_height;
	else if (top_y < 0)
		top_y = 0;

	// sanity check.
	if (bottom_y > level_height)
		bottom_y = level_height;
	else if (bottom_y < 0)
		bottom_y = 0;

	float top_col_percent = float(top_y) / float(level_height);
	float bottom_col_percent = float(bottom_y) / float(level_height);

	// compute the final top color
	int final_top_col = makecol(
									getr(bottom_col) + int(float(col_diff_r) * top_col_percent),
									getg(bottom_col) + int(float(col_diff_g) * top_col_percent),
									getb(bottom_col) + int(float(col_diff_b) * top_col_percent)
								);

	// compute the final bottom color
	int final_bottom_col = makecol(
									getr(bottom_col) +int(float(col_diff_r) * bottom_col_percent),
									getg(bottom_col) +int(float(col_diff_g) * bottom_col_percent),
									getb(bottom_col) +int(float(col_diff_b) * bottom_col_percent)
								);

	// draw the quad with the two new colors
	DrawQuad(	0, 0, width, height,
						final_bottom_col, final_bottom_col, final_top_col, final_top_col,
						true, 255);
}

// Colors start at the bottom left and go counter-clockwise
// Color order: (bottom left, bottom right, top right, top left)
void GameWindow::DrawQuad(	int x1, int y1, int x2, int y2, 
							int col1, int col2, int col3, int col4,
							bool filled, int alpha ) 
{
	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);

	if (filled)
		glBegin(GL_QUADS); 
	else
		glBegin(GL_LINES);

	glColor4ub(getr(col1), getg(col1), getb(col1), alpha);
	glVertex2f(x1, y1);
	glColor4ub(getr(col2), getg(col2), getb(col2), alpha);
	glVertex2f(x2, y1);
	glColor4ub(getr(col3), getg(col3), getb(col3), alpha);
	glVertex2f(x2, y2);
	glColor4ub(getr(col4), getg(col4), getb(col4), alpha);
	glVertex2f(x1, y2);

	glEnd();

	glEnable(GL_TEXTURE_2D);
	glColor4ub(255, 255, 255, 255);
}

void GameWindow::DrawRect(	int x1, int y1, 
														int x2, int y2, 
														int col, bool filled, int alpha) {

	DrawQuad(x1, y1, x2, y2, col, col, col, col, filled, alpha);
}

// HACK HACK HACK - get this from somewhere!!
#define FONT_HEIGHT 10

// TxtObject delimiters (see objectTxtOverlay.cpp)
#define OBJECT_TXT_LINE_DELIM "|"			// Which char goes to the next line

void GameWindow::DrawText(int x, int y, CString text) {
	vector<CString> lines;
	StringSplit(text, OBJECT_TXT_LINE_DELIM, lines);
	int i, max = lines.size();

	int _x = x;
	int _y = y;
	int col = makecol(255,255,255);	// white text color

	glLoadIdentity();
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
													bool flip_x, bool flip_y, 
													bool use_rotation, float rotate_angle,
													GLuint alpha) {
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
	glColor4ub(255,255,255,alpha);

	glLoadIdentity();

	glTranslatef(rx, ry, 0.0f);

	if (use_rotation) {
		glTranslatef(sprite->width/2.0f, sprite->height/2.0f, 0.0f);
		glRotatef(rotate_angle, 0.0f, 0.0f, 1.0f);
		glTranslatef(-sprite->width/2.0f, -sprite->height/2.0f, 0.0f);
	}

	glScalef(sprite->width, sprite->height, 1.0f);

	glBegin(GL_QUADS); 
    glTexCoord2f(tx1, ty1);
		glVertex2f(0,1);

    glTexCoord2f(tx2, ty2);
		glVertex2f(0,0);

    glTexCoord2f(tx3, ty3);
		glVertex2f(1,0);

    glTexCoord2f(tx4, ty4);
		glVertex2f(1,1);
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
	
	fade_rate = 0;
	fade_alpha = 255;
	fading_state = FADED_NONE;

	width = _width;
	height = _height;

	// Special case: We won't be drawing _anything_
	if (!OPTIONS->DrawGraphics() ) {
		TRACE("GameWindow: DISABLING ALL GRAPHICS\n");
		set_gfx_mode(GFX_TEXT, 320, 240, 0, 0);
		initialized = true;
		return 0;
	}

	install_allegro_gl();
	allegro_gl_clear_settings();
	
	set_color_depth(depth);

	uint gl_flags = AGL_COLOR_DEPTH | AGL_DOUBLEBUFFER | AGL_RENDERMETHOD;

	if (!_fullscreen) {
		gl_flags |= AGL_WINDOWED;
	} else {
		gl_flags |= AGL_FULLSCREEN;
	}

	allegro_gl_set(AGL_COLOR_DEPTH, depth);
	allegro_gl_set(AGL_DOUBLEBUFFER, 1);
	allegro_gl_set(AGL_Z_DEPTH, 24);
	allegro_gl_set(AGL_WINDOWED, !_fullscreen);
	allegro_gl_set(AGL_FULLSCREEN, _fullscreen);
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
		TRACE("window: Warning: Asked for %i-bit color mode"
										", got %i-bit instead.\n", depth, get_color_depth());
									
	if (set_gfx_mode(gfx_mode, width, height, 0, 0) != 0) {
		TRACE(
						"window: Can't set graphics mode! (%i, %i, fullscreen = %i) \n"
						"Try setting a different graphics mode or try non-fullscreen\n"
						"Allegro error says: '%s'\n"
						"AllegoGL error says: '%s'\n",
						width, height, _fullscreen, allegro_error, allegro_gl_error);
		return -1;
	}
	
	SetTitle(VERSION_STRING);

	// XXX: Font stuff should go in asset manager
	main_font = allegro_gl_convert_allegro_font(font, AGL_FONT_TYPE_TEXTURED, -1.0);
	assert(main_font != NULL);
	
	if (InitGL())
		initialized = true;
	else
		initialized = false;

	// Draw the window once so it's black whilst everything else loads
	BeginDrawing();
	Clear();
	DrawText(10, 10, "Pirates VS Ninjas Loading ...");
	Flip();
	EndDrawing();

	return 0;
}

void GameWindow::SetTitle(const char* szTitle)
{
	set_window_title(szTitle);
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

	if (main_font)
		allegro_gl_destroy_font(main_font);

	initialized = false;
}

GameWindow::GameWindow() : initialized(false) {
}

GameWindow::~GameWindow() {}
