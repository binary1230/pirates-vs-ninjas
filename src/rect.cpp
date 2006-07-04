#include "rect.h"

void Rect::fromVec(const Vector2D &v1, const Vector2D &v2) {
	set(v1.GetX(), v1.GetY(), v2.GetX(), v2.GetY());
}

Rect::Rect(float _x1, float _y1, float _x2, float _y2) {
	set(_x1, _y1, _x2, _y2);
}

Rect::Rect(const Vector2D &v1, const Vector2D &v2) {
	fromVec(v1, v2);
}

void Rect::Swap(float &t1, float &t2) {
	float t = t1; 
				t1 = t2; 
				t2 = t;
}
		
//! Make sure x1,y1 is the bottom left corner
//! and that x2,y2 is the top right corner
void Rect::Fix() {
	FixX();
	FixY();
}

void Rect::FixX() {
	if (x2<x1) Swap(x2,x1);
}

void Rect::FixY() {
	if (y2<y1) Swap(y2,y1);
}

void Rect::Print(char* name) {
	fprintf(stderr, "%s\t x1 = %f, y1 = %f, x2 = %f, y2 = %f\n", 
					name, x1, y1, x2, y2);
}

bool Rect::Overlaps(Rect rect) {	

	Fix();
	rect.Fix();

	//Print("ME:  ");
	// rect.Print("THEM:");

	if (y1 > rect.y2 ||
			y2 < rect.y1 ||
			x2 < rect.x1 ||
			x1 > rect.x2) 
		return false;

	return true;
}

Rect Rect::operator=(Rect r) {
			x1 = r.getx1();
			y1 = r.gety1();
			x2 = r.getx2();
			y2 = r.gety2();
			return *this;
}

Rect Rect::Project(Vector2D &projection) {

	Fix();

	Rect r = *this;
						
	if (projection.GetX() < 0)
		r.setx1(r.getx1() + projection.GetX());
	else
		r.setx2(r.getx2() + projection.GetX());
			
	if (projection.GetY() < 0)
		r.sety1(r.gety1() + projection.GetY());
	else
		r.sety2(r.gety2() + projection.GetY());

	return r;
}

//! Compare two rectangles
bool Rect::operator==(const Rect &r) const {
	return (x1 - r.getx1() < TOLERANCE && x1 - r.getx1() > -TOLERANCE &&
					y1 - r.gety1() < TOLERANCE && y1 - r.gety1() > -TOLERANCE &&
					x2 - r.getx2() < TOLERANCE && x2 - r.getx2() > -TOLERANCE &&
					y2 - r.gety2() < TOLERANCE && y2 - r.gety2() > -TOLERANCE );
}
		
//! Compare two rectangles
bool Rect::operator!=(const Rect &r) const {
	return (x1 - r.getx1() > TOLERANCE && x1 - r.getx1() < -TOLERANCE &&
					y1 - r.gety1() > TOLERANCE && y1 - r.gety1() < -TOLERANCE &&
					x2 - r.getx2() > TOLERANCE && x2 - r.getx2() < -TOLERANCE &&
					y2 - r.gety2() > TOLERANCE && y2 - r.gety2() < -TOLERANCE );
}


