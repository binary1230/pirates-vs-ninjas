#include "stdafx.h"
#include "rect.h"

/*void _Rect::fromVec(const Vector2D &v1, const Vector2D &v2) {
	set(v1.GetX(), v1.GetY(), v2.GetX(), v2.GetY());
}*/

_Rect::_Rect( const float &_x1, const float &_y1,
      			const float &_x2, const float &_y2 ) {
	set(_x1, _y1, _x2, _y2);
}

// copy constructor
_Rect::_Rect(const _Rect &r) {
	set(r.x1, r.y1, r.x2, r.y2);
}

/*_Rect::_Rect(const Vector2D &v1, const Vector2D &v2) {
	fromVec(v1, v2);
}*/

inline void Swap(register float &t1, register float &t2) {
	register float t;
	t = t1; 
	t1 = t2; 
	t2 = t;
}
		
//! Make sure x1,y1 is the bottom left corner
//! and that x2,y2 is the top right corner
/*void _Rect::Fix() {
	FixX();
	FixY();
}

void _Rect::FixX() {
	if (x2<x1) 
		Swap(x2,x1);
}

void _Rect::FixY() {
	if (y2<y1) 
		Swap(y2,y1);
}*/

void _Rect::Print(char* name) {
	TRACE("%s\t x1 = %f, y1 = %f, x2 = %f, y2 = %f\n", 
					name, x1, y1, x2, y2);
}

_Rect& _Rect::operator=(const _Rect &r) {
			x1 = r.x1;
			y1 = r.y1;
			x2 = r.x2;
			y2 = r.y2;
			return *this;
}

void _Rect::Project(const Vector2D &projection) {
	if (projection.GetX() >= 0.0f) {
		x2 += projection.GetX();	// GetX() is positive
	} else {
		x1 += projection.GetX();	// GetX() is negative
	}
			
	if (projection.GetY() >= 0.0f) {
		y2 += projection.GetY();	// GetY() is positive
	} else {
		y1 += projection.GetY();	// GetY() is negative
	}
}

//! Compare two rectangles
bool _Rect::operator==(const _Rect &r) const {
	return (getx1() - r.getx1() < TOLERANCE && getx1() - r.getx1() > -TOLERANCE &&
					gety1() - r.gety1() < TOLERANCE && gety1() - r.gety1() > -TOLERANCE &&
					getx2() - r.getx2() < TOLERANCE && getx2() - r.getx2() > -TOLERANCE &&
					gety2() - r.gety2() < TOLERANCE && gety2() - r.gety2() > -TOLERANCE );
}
		
//! Compare two rectangles
bool _Rect::operator!=(const _Rect &r) const {
	return (getx1() - r.getx1() > TOLERANCE && getx1() - r.getx1() < -TOLERANCE &&
					gety1() - r.gety1() > TOLERANCE && gety1() - r.gety1() < -TOLERANCE &&
					getx2() - r.getx2() > TOLERANCE && getx2() - r.getx2() < -TOLERANCE &&
					gety2() - r.gety2() > TOLERANCE && gety2() - r.gety2() < -TOLERANCE );
}

void _Rect::print(FILE* f) {
	fprintf(f, 	"+ + + + + + + + + + + + + + + + + + + + +\n"
							"++ x1,y1 = %f, %f\n"
							"++ x2,y2 = %f, %f\n"
							"+ + + + + + + + + + + + + + + + + + + + +\n",
							getx1(),gety1(),getx2(),gety2());
}
