// Object
// Base class for all objects displayable on the screen
// 
// TODO Replace BITMAP* sprite with animation class
#ifndef OBJECT_H
#define OBJECT_H

#include <allegro.h>

class Object {
				
	protected:
		int x, y;
		BITMAP* bitmap;				// this object's sprite
		BITMAP* dst_bitmap;		// which bitmap to Draw() to 

	public:
		void Update();				
		void Draw();
		
		void SetBitmap(BITMAP* _bitmap);
		void SetDestinationBitmap(BITMAP* _destination);

		Object();
		~Object();
};

#endif // OBJECT_H
