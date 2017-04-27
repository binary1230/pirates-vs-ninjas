#ifndef CAMERA_H
#define CAMERA_H

#include "globals.h"

class Camera
{
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version) {
		// noop
	}

	protected:
		b2Vec2 pos;

	public:
		virtual void Update();

		virtual void Init();

		virtual void GetXY(b2Vec2 & pos_out);

		Camera();
		virtual ~Camera();

		void ClampToLevelBounds();

		virtual void TransformWorldToView(int& x, int &y);
};

#endif // CAMERA_H