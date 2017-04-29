#include "stdafx.h"
#include "rect.h"

_Rect::_Rect( const float _x1, const float _y1,
      			const float _x2, const float _y2 ) {
	set(_x1, _y1, _x2, _y2);
}

// copy constructor
_Rect::_Rect(const _Rect &r) {
	set(r.x1, r.y1, r.x2, r.y2);
}

inline void Swap(register float &t1, register float &t2) {
	register float t;
	t = t1; 
	t1 = t2; 
	t2 = t;
}

void _Rect::Print(char* name) {
	TRACE("%s\t x1 = %f, y1 = %f, x2 = %f, y2 = %f\n", 
					name, x1, y1, x2, y2);
}

void _Rect::Project(const b2Vec2 &projection) {
	if (projection.x >= 0.0f) {
		x2 += projection.x;	// x is positive
	} else {
		x1 += projection.x;	// x is negative
	}
			
	if (projection.y >= 0.0f) {
		y2 += projection.y;	// y is positive
	} else {
		y1 += projection.y;	// y is negative
	}
}

//! Compare two rectangles
bool _Rect::operator==(const _Rect &r) const {
	return (x1 - r.x1 < TOLERANCE && x1 - r.x1 > -TOLERANCE &&
			y1 - r.y1 < TOLERANCE && y1 - r.y1 > -TOLERANCE &&
			x2 - r.x2 < TOLERANCE && x2 - r.x2 > -TOLERANCE &&
			y2 - r.y2 < TOLERANCE && y2 - r.y2 > -TOLERANCE );
}
		
//! Compare two rectangles
bool _Rect::operator!=(const _Rect &r) const {
	return (x1 - r.x1 > TOLERANCE && x1 - r.x1 < -TOLERANCE &&
			y1 - r.y1 > TOLERANCE && y1 - r.y1 < -TOLERANCE &&
			x2 - r.x2 > TOLERANCE && x2 - r.x2 < -TOLERANCE &&
			y2 - r.y2 > TOLERANCE && y2 - r.y2 < -TOLERANCE );
}

void _Rect::print(FILE* f) {
	fprintf(f, 	"+ + + + + + + + + + + + + + + + + + + + +\n"
							"++ x1,y1 = %f, %f\n"
							"++ x2,y2 = %f, %f\n"
							"+ + + + + + + + + + + + + + + + + + + + +\n",
							getx1(),gety1(),getx2(),gety2());
}
