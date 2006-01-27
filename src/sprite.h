#ifndef SPRITE_H
#define SPRITE_H

#include <allegro.h>

//! Drawable data with possible transparency and X,Y offsets
class Sprite {
	public:
		BITMAP* bmp;	
	
		int x_offset, y_offset;
		bool flip_x, flip_y;
		
		bool bitmap_is_deleteable;
	
		inline void Shutdown() {
			if (bitmap_is_deleteable)
				destroy_bitmap(bmp);

			bmp = NULL;
		}
};

#endif // SPRITE_H
