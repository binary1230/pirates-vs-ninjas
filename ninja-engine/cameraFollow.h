#ifndef CAMERA_FOLLOW_H
#define CAMERA_FOLLOW_H

#include "globals.h"
#include "camera.h"

class Object;

class CameraFollow : public Camera
{
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Camera);
		ar & boost::serialization::make_nvp("follow_object", m_pkCameraLookatTarget);
	}

	protected:
		//! Which object the camera should follow
		Object* m_pkCameraLookatTarget;

		//! Whether the camera is currently shaking or not
		bool m_bIsCameraShaking;

		int m_iCameraTotalShakeTime;
		int m_iCameraShakeTime;

		//! Camera threshold - how far it should slide before snapping
		// you can use this to make sure we're, say, 60 units from the sides
		// at all times.
		int m_iCameraSideMargins;

		//! Camera snap rate - how fast the camera should "snap" to new targets
		float m_fCameraSnapRate;

		int camera_shake_x;
		int camera_shake_y;

	public:
		virtual void GetXY(b2Vec2 & pos_out);
		virtual void TransformWorldToView(int & x, int & y);
		void SetCameraShake(bool state, int fadeout_time);
		void ComputeNewPosition();
		void ComputeNewShake();
		virtual void Update();

		virtual void SnapCamera();

		CameraFollow(Object* follow_target = nullptr);
		virtual ~CameraFollow();
		virtual void Init();

		inline Object* GetFollowTarget() { return m_pkCameraLookatTarget; }
};

#endif // CAMERA_FOLLOW_H