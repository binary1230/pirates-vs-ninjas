#include "stdafx.h"
#include "objectBackground.h"

#include "globals.h"
#include "gameState.h"
#include "animation.h"
#include "gameWorld.h"
#include "object.h"
#include "window.h"

void ObjectBackground::Update() {
	BaseUpdate();
	UpdateSimpleAnimations();
}

// We want to wrap the background around the level.  Compute the offset
// NOTE: Maybe we should really just move this code into the code
// common for any object.
void ObjectBackground::Draw() {

	int x, y, w, h, current_x, current_y, screen_w, screen_h;
	
	// TODO: Get From XML
	const bool tile_vertical = false;
	const bool tile_horizontal = true;
	
	h = GetHeight();
	w = GetWidth();

	x = _Pos.x;
	y = _Pos.y;
	screen_w = (int)WINDOW->Width();
	screen_h = (int)WINDOW->Height();

	assert(h > 0 && w > 0 && "Invalid (non-positive) dimension for bg image!\n");

	float fx = x; float fy = y;
	Transform(fx,fy);
	x = fx; y = fy;

	// For loop bodies
	#define TILE_VERTICAL 	current_y = (y % h) - h; \
													current_y < screen_h; \
													current_y += h
	#define TILE_HORIZONTAL current_x = (x % w) - w; \
													current_x < screen_w; \
													current_x += w

	if (!tile_vertical && !tile_horizontal) {

		// why even bother using a background object for this??
		WINDOW->DrawSprite(currentSprite, x, y, flip_x, flip_y);

	} else if (tile_vertical && !tile_horizontal) {

		for (TILE_VERTICAL) {
			WINDOW->DrawSprite(currentSprite, x, current_y, flip_x, flip_y);
		}

	} else if (!tile_vertical && tile_horizontal) {

		for (TILE_HORIZONTAL) {
			WINDOW->DrawSprite(currentSprite, current_x, y, flip_x, flip_y);
		}

	} else if (tile_vertical && tile_horizontal) {
		
		for (TILE_VERTICAL) {
			for (TILE_HORIZONTAL) {
				WINDOW->DrawSprite(currentSprite, current_x, current_y, flip_x, flip_y);
			}
		}

	}
}

bool ObjectBackground::Init() {
	return BaseInit();
}

void ObjectBackground::Shutdown() {
	BaseShutdown();
}

ObjectBackground::ObjectBackground() {}
ObjectBackground::~ObjectBackground() {}

BOOST_CLASS_EXPORT_GUID(ObjectBackground, "ObjectBackground")