#include "rect.h"

/*void Rect::fromVec(const Vector2D &v1, const Vector2D &v2) {
	set(v1.GetX(), v1.GetY(), v2.GetX(), v2.GetY());
}*/

Rect::Rect( const float &_x1, const float &_y1,
      			const float &_x2, const float &_y2 ) {
	set(_x1, _y1, _x2, _y2);
}

// copy constructor
Rect::Rect(const Rect &r) {
	set(r.x1, r.y1, r.x2, r.y2);
}

/*Rect::Rect(const Vector2D &v1, const Vector2D &v2) {
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
/*void Rect::Fix() {
	FixX();
	FixY();
}

void Rect::FixX() {
	if (x2<x1) 
		Swap(x2,x1);
}

void Rect::FixY() {
	if (y2<y1) 
		Swap(y2,y1);
}*/

void Rect::Print(char* name) {
	fprintf(stderr, "%s\t x1 = %f, y1 = %f, x2 = %f, y2 = %f\n", 
					name, x1, y1, x2, y2);
}

bool Rect::Overlaps(const Rect &rect) const {	
	if (gety1() > rect.gety2() 	||
			gety2() < rect.gety1() 	||
			getx2() < rect.getx1()	||
			getx1() > rect.getx2()	) 
		return false;

	return true;
}

Rect& Rect::operator=(const Rect &r) {
			x1 = r.x1;
			y1 = r.y1;
			x2 = r.x2;
			y2 = r.y2;
			return *this;
}

void Rect::Project(const Vector2D &projection) {
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
bool Rect::operator==(const Rect &r) const {
	return (getx1() - r.getx1() < TOLERANCE && getx1() - r.getx1() > -TOLERANCE &&
					gety1() - r.gety1() < TOLERANCE && gety1() - r.gety1() > -TOLERANCE &&
					getx2() - r.getx2() < TOLERANCE && getx2() - r.getx2() > -TOLERANCE &&
					gety2() - r.gety2() < TOLERANCE && gety2() - r.gety2() > -TOLERANCE );
}
		
//! Compare two rectangles
bool Rect::operator!=(const Rect &r) const {
	return (getx1() - r.getx1() > TOLERANCE && getx1() - r.getx1() < -TOLERANCE &&
					gety1() - r.gety1() > TOLERANCE && gety1() - r.gety1() < -TOLERANCE &&
					getx2() - r.getx2() > TOLERANCE && getx2() - r.getx2() < -TOLERANCE &&
					gety2() - r.gety2() > TOLERANCE && gety2() - r.gety2() < -TOLERANCE );
}

void Rect::print(FILE* f) {
	fprintf(f, 	"+ + + + + + + + + + + + + + + + + + + + +\n"
							"++ x1,y1 = %f, %f\n"
							"++ x2,y2 = %f, %f\n"
							"+ + + + + + + + + + + + + + + + + + + + +\n",
							getx1(),gety1(),getx2(),gety2());
}
