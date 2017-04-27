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
}

void Camera::GetXY(b2Vec2& pos_out) {
	// TODO make all this use vect not individual vars

	pos_out.x = pos.x;
	pos_out.y = pos.y;
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
	x -= pos.x;
	y -= pos.y;
}

Camera::Camera()
{
	pos.x = pos.y = 0.0f;
}

Camera::~Camera()
{
}


BOOST_CLASS_VERSION(Camera, 1)
BOOST_CLASS_EXPORT_GUID(Camera, "Camera")