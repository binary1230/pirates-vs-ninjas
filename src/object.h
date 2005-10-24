// Object
// Base class for all objects displayable on the screen
// 
// TODO Replace BITMAP* sprite with animation class
#ifndef OBJECT_H
#define OBJECT_H

class Object;

#include <allegro.h>
#include <stdio.h>

#include "globals.h"
#include "gameBase.h"

class Object : public GameBase {
				
	protected:
		int x, y;
		
		BITMAP* bitmap;						// this object's sprite
		bool bitmap_is_deleteable;	// whether to delete the sprite or not on deletion
		
		BITMAP* dst_bitmap;		// which bitmap to Draw() to 

	public:
		virtual bool Init(GameState* _game_state) = 0;
		virtual void Shutdown() = 0;
		
		virtual void Update() = 0;
		virtual void Draw() = 0;

		void DrawAtOffset(int x, int y);	// Draw this object at its coordinates
																			// plus this offset.
		
		void SetBitmap(BITMAP* _bitmap);
		void SetDestinationBitmap(BITMAP* _destination);
		
		void SetBitmapIsDeleteable(bool _is_deletable);
		bool GetBitmapIsDeleteable();

		int GetX();
		int GetY();
		virtual void SetX(int _x);
		virtual void SetY(int _y);
		virtual void SetXY(int _x, int _y);
		
		int GetWidth();
		int GetHeight();

		virtual void Delete();

		Object();
		virtual ~Object();
};

#endif // OBJECT_H
