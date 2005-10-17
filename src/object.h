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
		void DrawAtOffset(int x, int y);	// Draw this object at its coordinates
																			// plus this offset.
		
		void SetBitmap(BITMAP* _bitmap);
		void SetDestinationBitmap(BITMAP* _destination);

		int GetX();
		int GetY();

		Object();
		~Object();
};

#endif // OBJECT_H
