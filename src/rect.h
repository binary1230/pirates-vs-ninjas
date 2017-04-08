#ifndef RECT_H
#define RECT_H

class _Rect;

// NOTES:
// _Rect::getx,y methods are GUARANTEED to look like this:
// x1,y1 = bottom left
// x2,y2 = top right
//
// _Rect::setx,y methods will swap values things to make sure 
// (x1<x2) and (y1<y2)

//! A rectangle class, useful for bounding boxes/etc
class _Rect {
	private:
		float x1,y1,x2,y2;	
		
	public:
		inline float getx1() const { return x1; }; // left
		inline float gety1() const { return y1; }; // bottom
		inline float getx2() const { return x2; }; // right
		inline float gety2() const { return y2; }; // top

		// ehh... not exactly working
		/*inline void setx1(const float &_x1) {
			if (_x1 < x2) {
				x1 = _x1;
			} else {
				x1 = x2;
				x2 = _x1;
			}
		};

		inline void setx2(const float &_x2) {
			if (_x2 > x1) {
				x2 = _x2;
			} else {
				x2 = x1;
				x1 = _x2;
			}
		};

		inline void sety1(const float &_y1) {
			if (_y1 < y2) {
				y1 = _y1;
			} else {
				y1 = y2;
				y2 = _y1;
			}
		};

		inline void sety2(const float &_y2) {
			if (_y2 > y1) {
				y2 = _y2;
			} else {
				y2 = y1;
				y1 = _y2;
			}
		};*/

		inline void set(const float _x1, const float _y1, 
						const float _x2, const float _y2) {
			if (_x1 < _x2) {
				x1 = _x1; 
				x2 = _x2; 
			} else {
				x2 = _x1;
				x1 = _x2;
			}

			if (_y1 < _y2) {
				y1 = _y1; 
				y2 = _y2; 
			} else {
				y2 = _y1;
				y1 = _y2;
			}
		}

		//! Create a rect from 2 vectors
		// void fromVec(const Vector2D &v1, const Vector2D &v2);

		inline bool Overlaps(const _Rect rect) const {	
			return ( y1 <= rect.y2 && y2 >= rect.y1 && x2 >= rect.x1 && x1 <= rect.x2 );
		}
		
		void Print(char* name = "");

		//! Assign a rect's values to this rect
		_Rect& operator=(const _Rect &r) {
			x1 = r.x1;
			y1 = r.y1;
			x2 = r.x2;
			y2 = r.y2;
			return *this;
		}
		
		//! Use a vector to expand one of the corners of this
		//! rectangle.
		void Project(const b2Vec2 &projection);

		inline void Clear() {
			x1 = x2 = y1 = y2 = 0.0f;
		}
		
		//! Compare two rectangles
		bool operator==(const _Rect &v) const;
		bool operator!=(const _Rect &v) const;

		_Rect(	const float _x1, const float _y1, 
				const float _x2, const float _y2	);

		_Rect(const b2Vec2 &v1, const b2Vec2 &v2);
		_Rect(const _Rect &r);
		_Rect() { Clear(); };

		void print(FILE* f = stderr);
};

#endif // RECT_H
