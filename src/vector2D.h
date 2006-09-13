#ifndef VECTOR2D_H
#define VECTOR2D_H

#include "stdafx.h"
#include "globals.h"

using namespace std;

class Vector2D;

//! A 2 Dimensional Vector
class Vector2D {
	protected:
		float x, y;
		
	public:
		void Clear();

		inline float GetX() const {return x;}
	 	inline float GetY() const {return y;}
		inline void SetX(float _x) {x=_x;}
	 	inline void SetY(float _y) {y=_y;}
		
		//! Assign a vector's values to this vector
		inline Vector2D& operator=(const Vector2D &v) {
			x = v.GetX();
			y = v.GetY();
			return *this;
		}
	
		//! Add two vectors
		inline Vector2D operator+(const Vector2D &v) const {
			return Vector2D(x + v.GetX(), y + v.GetY());
		}

		//! Add another vector to this one
		inline void operator+=(const Vector2D &v) {
			x += v.GetX();
			y += v.GetY();
		}

		//! Subtract another vector from this one
		inline void operator-=(const Vector2D &v) {
			x -= v.GetX();
			y -= v.GetY();
		}

		//! Subtract two vectors
		inline Vector2D operator-(const Vector2D &v) const {
			return Vector2D(x - v.GetX(), y - v.GetY());
		}
		
		//! Compare two vectors
		inline bool operator==(const Vector2D &v) const {
			return (x - v.GetX() < TOLERANCE && x - v.GetX() > -TOLERANCE &&
							y - v.GetY() < TOLERANCE && y - v.GetY() > -TOLERANCE);
		}
		
		//! Compare two vectors
		inline bool operator!=(const Vector2D &v) const {
				return (x - v.GetX() > TOLERANCE || x - v.GetX() < -TOLERANCE ||
								y - v.GetY() > TOLERANCE || y - v.GetY() < -TOLERANCE);
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
			return (x * v.GetX()) + (y * v.GetY());
		}
		
		//! Compute the cross product
		/*inline Vector2D cross(Vector2D v) const {

		}*/

		//! Return a vector which has our components but negated
		inline Vector2D Negation() {
			return Vector2D(-GetX(), -GetY());
		}
		
		Vector2D();
		Vector2D(const float &_x, const float &_y);
		virtual ~Vector2D();
};

#endif // VECTOR2D_H
