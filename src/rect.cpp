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

