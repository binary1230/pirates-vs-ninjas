#include "stdafx.h"
#include "physicsDebugRenderer.h"
#include "physics.h"
#include "gameWorld.h"
#include "window.h"
#include "camera.h"

void PhysicsDebugRenderer::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_LINE_LOOP);
	for (int32 i = 0; i < vertexCount; ++i)
	{
		glVertex2f(vertices[i].x, vertices[i].y);
	}
	glEnd();
}

void PhysicsDebugRenderer::Transform(float &x, float &y)
{
	b2Vec2 cameraXY;
	WORLD->GetCamera()->GetXY(cameraXY);

	const float fCameraScrollSpeed = 1.0f; // TODO: Get from correct layer
	x = (x - cameraXY.x) * fCameraScrollSpeed;
	y = (y - cameraXY.y) * fCameraScrollSpeed;

	y = WINDOW->Height() - y;
}

void PhysicsDebugRenderer::SubmitVertex(float x, float y)
{
	x = METERS_TO_PIXELS(x);
	y = METERS_TO_PIXELS(y);

	Transform(x, y);
	glVertex2f(x, y);
}

void PhysicsDebugRenderer::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
	// This is correct for Pirates VS Ninjas engine.

	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.75f);

	glBegin(GL_TRIANGLE_FAN);

	for (int i = 0; i < vertexCount; ++i)
		SubmitVertex(vertices[i].x, vertices[i].y);

	glEnd();
	glDisable(GL_BLEND);

	glColor4f(color.r, color.g, color.b, 1.0f);
	glBegin(GL_LINE_LOOP);

	for (int i = 0; i < vertexCount; ++i)
		SubmitVertex(vertices[i].x, vertices[i].y);

	glEnd();

	glEnable(GL_TEXTURE_2D);
}

void PhysicsDebugRenderer::DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color)
{
	assert(0 && "TODO: Implement this for pirates vs ninjas engine, it won't work right now.");

	const float32 k_segments = 16.0f;
	const float32 k_increment = 2.0f * b2_pi / k_segments;
	float32 theta = 0.0f;
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_LINE_LOOP);
	for (int32 i = 0; i < k_segments; ++i)
	{
		b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
		glVertex2f(v.x, v.y);
		theta += k_increment;
	}
	glEnd();
}

void PhysicsDebugRenderer::DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color)
{
	const float32 k_segments = 16.0f;
	const float32 k_increment = 2.0f * b2_pi / k_segments;
	float32 theta = 0.0f;

	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0.5f * color.r, 0.5f * color.g, 0.5f * color.b, 0.5f);
	glBegin(GL_TRIANGLE_FAN);
	for (int32 i = 0; i < k_segments; ++i)
	{
		b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
		glVertex2f(v.x, v.y);
		theta += k_increment;
	}
	glEnd();
	glDisable(GL_BLEND);

	theta = 0.0f;
	glColor4f(color.r, color.g, color.b, 1.0f);
	glBegin(GL_LINE_LOOP);
	for (int32 i = 0; i < k_segments; ++i)
	{
		b2Vec2 v = center + radius * b2Vec2(cosf(theta), sinf(theta));
		glVertex2f(v.x, v.y);
		theta += k_increment;
	}
	glEnd();

	b2Vec2 p = center + radius * axis;
	glBegin(GL_LINES);
	glVertex2f(center.x, center.y);
	glVertex2f(p.x, p.y);
	glEnd();

	glEnable(GL_TEXTURE_2D);
}

void PhysicsDebugRenderer::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	assert(0 && "TODO: Implement this for pirates vs ninjas.");

	glColor3f(color.r, color.g, color.b);
	glBegin(GL_LINES);
	glVertex2f(p1.x, p1.y);
	glVertex2f(p2.x, p2.y);
	glEnd();
}

void PhysicsDebugRenderer::DrawTransform(const b2Transform& xf)
{
	assert(0 && "TODO: Implement this for pirates vs ninjas.");

	/*b2Vec2 p1 = xf.p, p2;
	const float32 k_axisScale = 0.4f;
	glBegin(GL_LINES);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(p1.x, p1.y);
	p2 = p1 + k_axisScale * xf.q.col1;
	glVertex2f(p2.x, p2.y);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex2f(p1.x, p1.y);
	p2 = p1 + k_axisScale * xf.q.col2;
	glVertex2f(p2.x, p2.y);

	glEnd();*/
}

void PhysicsDebugRenderer::DrawPoint(const b2Vec2 & p, float32 size, const b2Color & color)
{
	assert(0 && "TODO: Implement this for pirates vs ninjas.");
}

void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
{
	assert(0 && "TODO: Implement this for pirates vs ninjas.");

	glPointSize(size);
	glBegin(GL_POINTS);
	glColor3f(color.r, color.g, color.b);
	glVertex2f(p.x, p.y);
	glEnd();
	glPointSize(1.0f);
}

void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
	assert(0 && "TODO: Implement this for pirates vs ninjas.");

	glColor3f(color.r, color.g, color.b);
	glBegin(GL_LINES);
	glVertex2f(p1.x, p1.y);
	glVertex2f(p2.x, p2.y);
	glEnd();
}

void DrawAABB(b2AABB* aabb, const b2Color& c)
{
	assert(0 && "TODO: Implement this for pirates vs ninjas.");

	glColor3f(c.r, c.g, c.b);
	glBegin(GL_LINE_LOOP);
	glVertex2f(aabb->lowerBound.x, aabb->lowerBound.y);
	glVertex2f(aabb->upperBound.x, aabb->lowerBound.y);
	glVertex2f(aabb->upperBound.x, aabb->upperBound.y);
	glVertex2f(aabb->lowerBound.x, aabb->upperBound.y);
	glEnd();
}