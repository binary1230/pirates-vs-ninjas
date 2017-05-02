#include "stdafx.h"
#include "camera.h"
#include "globals.h"
#include "gameWorld.h"
#include "window.h"

void Camera::Update()
{
}

void Camera::Init()
{
	camera_offset = b2Vec2(0, 0);
}

void Camera::GetXY(b2Vec2& pos_out) {
	pos_out.x = pos.x + camera_offset.x;
	pos_out.y = pos.y + camera_offset.y;
}

void Camera::ClampToLevelBounds() {

	int levelWidth = WORLD->GetWidth();
	int levelHeight = WORLD->GetHeight();

	int sw = WINDOW->Width();
	int sh = WINDOW->Height();

	if (pos.x < 0) 
		pos.x = 0;

	if (pos.x > levelWidth - sw) 
		pos.x = levelWidth - sw;

	if (pos.y < 0) 
		pos.y = 0;

	if (pos.y > levelHeight - sh) 
		pos.y = levelHeight - sh;
}

void Camera::TransformWorldToView(int & x, int & y)
{
	x -= pos.x + camera_offset.x;
	y -= pos.y + camera_offset.y;
}

void Camera::SetCameraOffset(b2Vec2 & offset)
{
	camera_offset = offset;
}

Camera::Camera()
{
	camera_offset = pos = b2Vec2(0, 0);
}

Camera::~Camera()
{
}


BOOST_CLASS_VERSION(Camera, 1)
BOOST_CLASS_EXPORT_GUID(Camera, "Camera")