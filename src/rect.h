#ifndef RECT_H
#define RECT_H

#include "stdafx.h"
#include "vector2D.h"

class Rect;

//! A rectangle class, useful for bounding boxes/etc
// x1,y1 = bottom left
// x2,y2 = top right
class Rect {
	private:
		float x1,y1,x2,y2;	
		
		//! Make sure x1,y1 is the top left corner
		void FixX();
		void FixY();

	public:
		void Fix();

		inline float getx1() const {return x1;};
		inline float gety1() const {return y1;};
		inline float getx2() const {return x2;};
		inline float gety2() const {return y2;};

		inline void setx1(float _x1) {x1=_x1;};
		inline void sety1(float _y1) {y1=_y1;};
		inline void setx2(float _x2) {x2=_x2;};
		inline void sety2(float _y2) {y2=_y2;};
		inline void setx1y1(float _x1, float _y1) {setx1(_x1); sety1(_y1);};
		inline void setx2y2(float _x2, float _y2) {setx2(_x2); sety2(_y2);};

		inline void set(float _x1, float _y1, float _x2, float _y2) {
			x1 = _x1; y1 = _y1;	x2 = _x2; y2 = _y2; 
			Fix();
		}

		//! Create a rect from 2 vectors
		void fromVec(const Vector2D &v1, const Vector2D &v2);

		bool Overlaps(Rect rect);
		
		void Print(char* name = "");

		//! Assign a rect's values to this rect
		Rect Rect::operator=(Rect r);
		
		Rect Project(Vector2D &projection) const;

		inline void Clear() {
			x1 = x2 = y1 = y2 = 0.0f;
		}
		
		//! Compare two rectangles
		bool Rect::operator==(const Rect &v) const;
		bool Rect::operator!=(const Rect &v) const;

		Rect(float _x1 =0.0f, float _y1 =0.0f, float _x2 =0.0f, float _y2 =0.0f);
		Rect(const Vector2D &v1, const Vector2D &v2);
		Rect(const Rect &r);

		void print(FILE* f = stderr);
};

#endif // RECT_H
