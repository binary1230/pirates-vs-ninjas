#ifndef VECTOR2D_H
#define VECTOR2D_H

#include "stdafx.h"
#include "globals.h"

typedef b2Vec2 Vector2D;

/*class Vector2D : public b2Vec2
{

};*/

//! A 2 Dimensional Vector
/*class Vector2D {		
	public:		
		float x, y;

		//! Assign a vector's values to this vector
		inline Vector2D& operator=(const Vector2D &v) {
			x = v.x;
			y = v.y;
			return *this;
		}
	
		//! Add two vectors
		inline Vector2D operator+(const Vector2D &v) const {
			return Vector2D(x + v.x, y + v.y);
		}

		//! Add another vector to this one
		inline void operator+=(const Vector2D &v) {
			x += v.x;
			y += v.y;
		}

		//! Subtract another vector from this one
		inline void operator-=(const Vector2D &v) {
			x -= v.x;
			y -= v.y;
		}

		//! Subtract two vectors
		inline Vector2D operator-(const Vector2D &v) const {
			return Vector2D(x - v.x, y - v.y);
		}
		
		//! Compare two vectors
		inline bool operator==(const Vector2D &v) const {
			return (x - v.x < TOLERANCE && x - v.x > -TOLERANCE &&
					y - v.y < TOLERANCE && y - v.y > -TOLERANCE);
		}
		
		//! Compare two vectors
		inline bool operator!=(const Vector2D &v) const {
				return (x - v.x > TOLERANCE || x - v.x < -TOLERANCE ||
						y - v.y > TOLERANCE || y - v.y < -TOLERANCE);
		}

		//! Scalar product (vector * number)
		inline Vector2D operator*(const float &n) const {
			return Vector2D(x*n, y*n);
		}
		
		//! Subtract another vector from this one
		inline void operator*=(const float &n) {
			x *= n;
			y *= n;
		}


		//! Scalar divide (vector * number)
		//XXX need to add exception handling for DIVIDE by ZERO
		inline Vector2D operator/(const float &n) const {
			return Vector2D(x/n, y/n);
		}

		//! Compute the dot product
		inline float Dot(const Vector2D &v) const {
			return (x * v.x) + (y * v.y);
		}

		//! Return a vector which has our components but negated
		inline Vector2D Negation() {
			return Vector2D(-x, -y);
		}

		inline float LengthSquared() {
			return (x * x) + (y * y);
		}

		inline float Length() {
			return fsqrt((x * x) + (y * y));
		}
		
		Vector2D() : x(0.0f), y(0.0f) {}
		Vector2D(const float _x, const float _y) : x(_x), y(_y) {}

		~Vector2D() {}

		inline void Clear() {
			x = y = 0.0f;
		}
};*/

#endif // VECTOR2D_H
