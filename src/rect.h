#ifndef RECT_H
#define RECT_H

#include "vector2D.h"

#include <stdio.h>
#include <math.h>
#include <vector>

using namespace std;

class Rect;

//! A rectangle class, useful for bounding boxes/etc
class Rect {
	private:
		float x1,y1,x2,y2;	
		
		inline void Swap(float &t1, float &t2) {
			float t = t1; t1 = t2; t2 = t;
		}
		
		//! Make sure x1,y1 is the top left corner
		inline void Fix() {
			if (x2<x1) Swap(x2,x1);
			if (y1<y2) Swap(y2,y1);
		}

	public:
		inline float const getx1() {return x1;};
		inline float const gety1() {return y1;};
		inline float const getx2() {return x2;};
		inline float const gety2() {return y2;};

		inline void setx1(float _x1) {x1=_x1; Fix();};
		inline void sety1(float _y1) {y2=_y1; Fix();};
		inline void setx2(float _x2) {x1=_x2; Fix();};
		inline void sety2(float _y2) {y2=_y2; Fix();};
		inline void setx1y1(float _x1, float _y1) {setx1(_x1); sety1(_y1);};
		inline void setx2y2(float _x2, float _y2) {setx2(_x2); sety2(_y2);};

		inline void set(float _x1, float _y1, float _x2, float _y2) {
			x1 = _x1; y1 = _y1;	x2 = x2; y2 = _y2; 
			Fix();
		}

		//! Create a rect from 2 vectors
		void fromVec(const Vector2D &v1, const Vector2D &v2);

		inline bool const Overlaps(const Rect &rect) {	
			// remember, x1 is always < x2
			// and y1 is always > y2
			if ( 	(
							(rect.x1 <= x1 && rect.x2 >= x1) || 
							(rect.x1 <= x2 && rect.x2 >= x2) 
						) && (
							(rect.y1 >= y1 && rect.y2 <= y1) || 
							(rect.y1 >= y2 && rect.y2 <= y2)  
						) )
				return true;
			else
				return false;
		}

		//! Assign a rect's values to this rect
		Rect Rect::operator=(Rect r) {
			x1 = r.getx1();
			y1 = r.gety1();
			x2 = r.getx2();
			y2 = r.gety2();
			return *this;
		}

		// projRect = bbox.Project(projection);
		inline Rect Project(Vector2D &projection) {
			Rect r = *this;
						
			if (projection.GetX() < 0)
				r.setx1(r.getx1() - projection.GetX());
			else
				r.setx2(r.getx2() + projection.GetX());
			
			if (projection.GetY() > 0)
				r.sety1(r.gety1() + projection.GetY());
			else
				r.sety2(r.gety2() - projection.GetY());

			return r;
		}

		Rect(float _x1 =0.0f, float _y1 =0.0f, float _x2 =0.0f, float _y2 =0.0f);
		Rect(const Vector2D &v1, const Vector2D &v2);
};

#endif // RECT_H
