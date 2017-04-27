#include "stdafx.h"
#include "cameraFollow.h"
#include "globals.h"
#include "cameraFollow.h"
#include "object.h"
#include "gameWorld.h"
#include "window.h"
#include "globalDefines.h"

#define CAMERA_SHAKE false
#define CAMERA_SHAKE_X_MAGNITUDE 15
#define CAMERA_SHAKE_Y_MAGNITUDE 15

//! Weighted average nums for the 'floaty' camera
//! Increase CAM_WEIGHT_CAM to make the camera 'snap' quicker
#define CAM_WEIGHT_POS 1.0f // DON'T CHANGE.

//! Function which moves the camera according to a weight, shown above
//! Uses a weighted average of the object coordinates and the new camera coords
#define CAM_MOVE_TO_CENTER(cam, o, o_size, s_size) 									\
	int( float( 																											\
			(((o + o_size / 2.0f) - (s_size / 2.0f)) * CAM_WEIGHT_POS) + 	\
			((cam) * m_fCameraSnapRate) 																			\
		) / (m_fCameraSnapRate + CAM_WEIGHT_POS) )


void CameraFollow::GetXY(b2Vec2& pos_out) {
	Camera::GetXY(pos_out);

	pos_out.x += camera_shake_x;
	pos_out.y += camera_shake_y;
}

void CameraFollow::SetCameraShake(bool state, int fadeout_time) {
	m_iCameraShakeTime = 0;
	m_iCameraTotalShakeTime = fadeout_time;
	m_bIsCameraShaking = state;

	if (!m_bIsCameraShaking)
		m_iCameraTotalShakeTime = -1;
}

void CameraFollow::ComputeNewPosition() {
	assert(m_pkCameraLookatTarget != NULL);

	int ox = m_pkCameraLookatTarget->GetX();
	int ow = m_pkCameraLookatTarget->GetWidth();
	int sw = WINDOW->Width();

	int oy = m_pkCameraLookatTarget->GetY();
	int oh = m_pkCameraLookatTarget->GetHeight();
	int sh = WINDOW->Height();

	// compute the next interpolated position

	pos.x = CAM_MOVE_TO_CENTER(pos.x, ox, ow, sw);
	pos.y = CAM_MOVE_TO_CENTER(pos.y, oy, oh, sh);

	// keep it within a certain margin of the sides
	if (ox - pos.x < m_iCameraSideMargins)
		pos.x = ox - m_iCameraSideMargins;
	else if ((pos.x + sw) - (ox + ow) < m_iCameraSideMargins)
		pos.x = ox + ow + m_iCameraSideMargins - sw;

	if (oy - pos.y < m_iCameraSideMargins)
		pos.y = oy - m_iCameraSideMargins;
	else if ((pos.y + sh) - (oy + oh) < m_iCameraSideMargins)
		pos.y = oy + oh + m_iCameraSideMargins - sh;

	ClampToLevelBounds();
}

void CameraFollow::ComputeNewShake() {

	// do the camera shake
	if (!m_bIsCameraShaking) {
		camera_shake_x = 0;
		camera_shake_y = 0;
		return;
	}
	
	float multiplier = 1.0f;

	if (m_iCameraTotalShakeTime != -1) {
		if (m_iCameraShakeTime >= m_iCameraTotalShakeTime) {
			m_bIsCameraShaking = false;
			multiplier = 0.0f;
		} else {
			++m_iCameraShakeTime;
			multiplier = 1.0f - float(m_iCameraShakeTime) / float(m_iCameraTotalShakeTime);
		}
	}

	camera_shake_x = Rand(0, float(CAMERA_SHAKE_X_MAGNITUDE) * multiplier);
	camera_shake_y = Rand(0, float(CAMERA_SHAKE_Y_MAGNITUDE) * multiplier);
}

void CameraFollow::Update()
{
	Camera::Update();

	ComputeNewPosition();
	ComputeNewShake();
}

void CameraFollow::Init() {
	Camera::Init();

	GLOBALS->Value("camera_side_margins", m_iCameraSideMargins);
	GLOBALS->Value("camera_snap_rate", m_fCameraSnapRate);

	SnapCamera();
}


// Snap the camera to its target object
// Useful when switching targets
void CameraFollow::SnapCamera() {

	assert(m_pkCameraLookatTarget);

	// center the camera on this object
	pos.x = int(
		(
			float(m_pkCameraLookatTarget->GetX()) +
			(float(m_pkCameraLookatTarget->GetWidth()) / 2.0f)
			) - (
			(float(WINDOW->Height()) / 2.0f)
				));

	pos.y = int(
		(
			float(m_pkCameraLookatTarget->GetY()) +
			(float(m_pkCameraLookatTarget->GetHeight()) / 2.0f)
			) - (
			(float(WINDOW->Width()) / 2.0f)
				));

	// Update(); // prob not needed.
}

CameraFollow::CameraFollow(Object* follow_target)
{
	Camera::Camera();

	m_pkCameraLookatTarget = follow_target;
	
	camera_shake_x = 0;
	camera_shake_y = 0;

	m_bIsCameraShaking = CAMERA_SHAKE;
	m_iCameraTotalShakeTime = -1;
	m_iCameraShakeTime = 0;
	m_iCameraSideMargins = 40;
	m_fCameraSnapRate = 3.0f;
}

CameraFollow::~CameraFollow()
{
}

BOOST_CLASS_VERSION(CameraFollow, 1)
BOOST_CLASS_EXPORT_GUID(CameraFollow, "CameraFollow")