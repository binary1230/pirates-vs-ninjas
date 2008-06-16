#include "stdafx.h"
#include "physics.h"
#include "gameWorld.h"
#include "window.h"
#include "object.h"

// NOTE: do NOT new/delete ANY physics objects EXCEPT m_pkWorld
// 50 pixels == 1 meter in physics here

DECLARE_SINGLETON(PhysicsManager)

PhysicsManager::PhysicsManager()
{
	m_fPhysicsSimulatorTimeStep = -1.0f;
	m_iPhysicsSimulatorIterations = -1;
	m_pkPhysicsWorld = NULL;
}

PhysicsManager::~PhysicsManager() {}

// Happens _after_ level is loaded
bool PhysicsManager::OnWorldInit()
{
	assert(WORLD);
	if (!WORLD)
		return false;

	assert(WORLD->GetWidth() > 0 && WORLD->GetHeight() > 0);
	float fWidthInMeters = PIXELS_TO_METERS(WORLD->GetWidth());
	float fHeightInMeters = PIXELS_TO_METERS(WORLD->GetHeight());

	// Define the size of the world. Simulation will still work
	// if bodies reach the end of the world, but it will be slower.
	b2AABB worldAABB;
	worldAABB.lowerBound.Set(-50.0f, -50.0f);
	worldAABB.upperBound.Set(fWidthInMeters + 50.0f, fHeightInMeters + 50.0f);

	b2Vec2 gravity(0.0f, -15.0f);
	bool doSleep = true;

	assert(m_pkPhysicsWorld == NULL);
	m_pkPhysicsWorld = new b2World(worldAABB, gravity, doSleep);

	// NOTE FROM DOM: If you are getting errors here, it's because I hacked the source
	// of Box2D to 1) make SetDebugDraw() public, and 2) remove the call from b2World::Step()
	m_kPhysicsDebugRenderer.SetFlags(PhysicsDebugRenderer::e_shapeBit);
	m_pkPhysicsWorld->SetDebugDraw(&m_kPhysicsDebugRenderer);

	m_pkPhysicsWorld->SetContactListener(&m_kPhysicsContactListener);

	return true;
}

// Happens before level load
bool PhysicsManager::Init()
{
	m_fPhysicsSimulatorTimeStep = 1.0f / FPS;
	m_iPhysicsSimulatorIterations = 10;
	bDrawDebugBoxes = false;

	return true;
}

void PhysicsManager::Update()
{
	m_kContacts.clear();

	// Instruct the world to perform a single step of simulation. It is
	// generally best to keep the time step and iterations fixed.
	m_pkPhysicsWorld->Step(m_fPhysicsSimulatorTimeStep, m_iPhysicsSimulatorIterations);

	// dispatch collision events now.
	HandleCollisions();
}

void PhysicsManager::Shutdown()
{
	SAFE_DELETE(m_pkPhysicsWorld);
}

void PhysicsManager::Draw()
{
	if (bDrawDebugBoxes)
		m_pkPhysicsWorld->DrawDebugData();
}

b2Body* PhysicsManager::CreatePhysicsBox( float x, float y, float width, float height, float density, float restitution, float friction, bool bDontAllowRotation /*= false */ )
{
	b2BodyDef bodyDef;
	b2PolygonDef shapeDef;

	assert(m_pkPhysicsWorld);
	assert(width > 0.0f);
	assert(height > 0.0f);

	float halfWidth = PIXELS_TO_METERS(width) / 2;
	float halfHeight = PIXELS_TO_METERS(height) / 2;

	bodyDef.position.Set(PIXELS_TO_METERS(x) + halfWidth, PIXELS_TO_METERS(y) + halfHeight);
	bodyDef.fixedRotation = bDontAllowRotation;
	//bodyDef.linearDamping = 0.4f;

	b2Body* pkBody = m_pkPhysicsWorld->CreateBody(&bodyDef);
	assert(pkBody);

	shapeDef.SetAsBox(halfWidth, halfHeight);
	shapeDef.friction = friction;
	shapeDef.restitution = restitution;
	shapeDef.density = density;

	pkBody->CreateShape(&shapeDef);

	return pkBody;
}


b2Body* PhysicsManager::CreateStaticPhysicsBox( float x, float y, float width, float height )
{
	return CreatePhysicsBox(x,y,width,height, 0.0f, 0.0f, 0.2f);
}

b2Body* PhysicsManager::CreateDynamicPhysicsBox( float x, float y, float width, float height, bool bDontAllowRotation )
{
	// TODO: Don't hardcode these numbers.
	b2Body* pkBody = CreatePhysicsBox(x,y,width,height, 0.1f, 0.0f, 0.2f, bDontAllowRotation);
	pkBody->SetMassFromShapes();
	return pkBody;
}

void PhysicsManager::RemoveFromWorld( b2Body* pkBodyToRemove )
{
	assert(m_pkPhysicsWorld);
	m_pkPhysicsWorld->DestroyBody(pkBodyToRemove);
}

void PhysicsManager::HandleCollisions()
{
	uint iMax = m_kContacts.size();

	for (uint i = 0; i < iMax; ++i)
	{
		ProcessCollision(&m_kContacts[i]);
	}
}

void PhysicsManager::ProcessCollision(b2ContactPoint* pkContactPoint)
{
	Object* obj1 = (Object*)pkContactPoint->shape1->GetBody()->GetUserData();
	Object* obj2 = (Object*)pkContactPoint->shape2->GetBody()->GetUserData();
	
	obj2->OnCollide(obj1, pkContactPoint);
	pkContactPoint->normal = -pkContactPoint->normal;
	obj1->OnCollide(obj2, pkContactPoint);
}

void PhysicsManager::ReportContactPoint( const b2ContactPoint* pkContactPoint )
{
	// make a COPY here, don't store the pointer.
	m_kContacts.push_back(*pkContactPoint);
}

// -------------------------------------------------------------------------

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

void PhysicsDebugRenderer::Transform( float &x, float &y )
{
	const float fCameraScrollSpeed = 1.0f; // TODO: Get from correct layer
	x = (x - WORLD->GetCameraX()) * fCameraScrollSpeed;
	y = (y - WORLD->GetCameraY()) * fCameraScrollSpeed;

	y = WINDOW->Height() - y;
}

void PhysicsDebugRenderer::SubmitVertex( float x, float y )
{
	x = METERS_TO_PIXELS(x); 
	y = METERS_TO_PIXELS(y);

	Transform(x, y);
	glVertex2f(x,y);
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

	for (int i = 0; i < 4; ++i)
		SubmitVertex(vertices[i].x, vertices[i].y);

	glEnd();
	glDisable(GL_BLEND);

	glColor4f(color.r, color.g, color.b, 1.0f);
	glBegin(GL_LINE_LOOP);

	for (int i = 0; i < 4; ++i)
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
	assert(0 && "TODO: Implement this for pirates vs ninjas.");

	const float32 k_segments = 16.0f;
	const float32 k_increment = 2.0f * b2_pi / k_segments;
	float32 theta = 0.0f;
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

void PhysicsDebugRenderer::DrawXForm(const b2XForm& xf)
{
	assert(0 && "TODO: Implement this for pirates vs ninjas.");

	b2Vec2 p1 = xf.position, p2;
	const float32 k_axisScale = 0.4f;
	glBegin(GL_LINES);

	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex2f(p1.x, p1.y);
	p2 = p1 + k_axisScale * xf.R.col1;
	glVertex2f(p2.x, p2.y);

	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex2f(p1.x, p1.y);
	p2 = p1 + k_axisScale * xf.R.col2;
	glVertex2f(p2.x, p2.y);

	glEnd();
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

void PhysicsContactListener::Add( const b2ContactPoint* point )
{
	// TRACE("%i: Add\n", i);
	PHYSICS->ReportContactPoint(point);
}

void PhysicsContactListener::Persist( const b2ContactPoint* point )
{
	// TRACE("%i: Persist\n", i);
	PHYSICS->ReportContactPoint(point);
}

void PhysicsContactListener::Remove( const b2ContactPoint* point )
{
}

void PhysicsContactListener::Result( const b2ContactResult* point )
{
}
