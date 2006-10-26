#ifndef SPRITE_H
#define SPRITE_H

#include "stdafx.h"

//! Drawable data with possible transparency and X,Y offsets
class Sprite {
	public:
		BITMAP* bmp;	
	
		int x_offset, y_offset;
		bool flip_x, flip_y;
		bool use_alpha;

		GLuint texture;	// only used for 3d

		Sprite() {
			x_offset = y_offset = 0;
			flip_x = flip_y = false;
			use_alpha = false;
			texture = 0;
		}
};

#endif // SPRITE_H
