#include "stdafx.h"
#include "physics.h"
#include "gameWorld.h"
#include "window.h"
#include "object.h"
#include "physicsDebugRenderer.h"
#include "gameState.h"
#include "physicsExplosion.h"

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

	b2Vec2 gravity(0.0f, -20.0f);

	assert(m_pkPhysicsWorld == NULL);
	m_pkPhysicsWorld = new b2World(gravity);

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

	return true;
}

void PhysicsManager::Update()
{
	int32 velocityIterations = 6;  // TEMP TODO

	// Instruct the world to perform a single step of simulation. It is
	// generally best to keep the time step and iterations fixed.
	m_pkPhysicsWorld->Step(m_fPhysicsSimulatorTimeStep, velocityIterations, m_iPhysicsSimulatorIterations);

	for (PhysicsExplosion*& explosion : _explosions) {
		explosion->Update();
	}

	// dispatch collision events now.
	ProcessCollisions();
}

void PhysicsManager::Shutdown()
{
	SAFE_DELETE(m_pkPhysicsWorld);
	m_currentContacts.clear();
}

void PhysicsManager::CreateExplosionAt(const b2Vec2 & center)
{
	PhysicsExplosion* explosion = nullptr;
	for (PhysicsExplosion*& e : _explosions) {
		if (e->IsDead()) {
			explosion = e;
			break;
		}
	}

	if (!explosion) {
		explosion = new PhysicsExplosion();
		_explosions.push_back(explosion);
	}

	explosion->ExplodeAt(center);
}

void PhysicsManager::Draw()
{
	if (GAMESTATE->GetPropPhysicsDebugDraw()) {
		m_pkPhysicsWorld->DrawDebugData();
		
		for (PhysicsExplosion*& explosion : _explosions) {
			explosion->DebugDraw();
		}
	}
}

void PhysicsManager::CreatePolygonWithRoundedEdges(float hx, float hy, b2PolygonShape& shapeOut) {
	/*
	
	create a polygon with angled corners.
	this is useful for preventing erroneous collisions for player characters
	shape will look like this, with angled bottom corners:

    F              E
	----------------
	|              |
	|A      X      |D
	 \            /
	  B--------- C

	hx = half of the width of the box,  from the center point X
	hy = half of the height of the box, from the center point X

	*/

	float percent_x_inwards = 0.8f;
	float percent_y_upwards = 0.95f;

	const int32 count = 6;
	b2Vec2 vertices[count];

	// original corner of -hx, -hy, now made into an angle
	vertices[0].Set(-hx, -hy * percent_y_upwards);	// A
	vertices[1].Set(-hx * percent_x_inwards, -hy);	// B

	// original corner of hx, -hy, now made into an angle
	vertices[2].Set(hx * percent_x_inwards, -hy);	// C
	vertices[3].Set(hx, -hy * percent_y_upwards);	// D

	// top corners, unmodified from box
	vertices[4].Set(hx, hy);	// E
	vertices[5].Set(-hx, hy);	// F

	shapeOut.Set(vertices, count);
}

void PhysicsManager::UpdatePhysicsBodyPosition(b2Body* body, float x, float y, int width, int height) {
	assert(body);
	
	float halfWidth = PIXELS_TO_METERS(width) / 2;
	float halfHeight = PIXELS_TO_METERS(height) / 2;

	body->SetTransform(b2Vec2(PIXELS_TO_METERS(x) + halfWidth, PIXELS_TO_METERS(y) + halfHeight), body->GetAngle());
}

b2Body* PhysicsManager::CreatePhysicsBox( float x, float y, float width, float height, float density, float restitution, float friction, bool bDontAllowRotation /*= false */, bool bSensorOnly /*= false*/, bool useRoundedBottom, PhysicsCategory category)
{
	b2BodyDef bodyDef;

	assert(m_pkPhysicsWorld);
	assert(width > 0.0f);
	assert(height > 0.0f);

	bodyDef.fixedRotation = bDontAllowRotation;
	// bodyDef.linearDamping = 2.0f;

	if (density > 0.0f)
		bodyDef.type = b2_dynamicBody;

	b2Body* pkBody = m_pkPhysicsWorld->CreateBody(&bodyDef);
	assert(pkBody);

	UpdatePhysicsBodyPosition(pkBody, x, y, width, height);

	float halfWidth = PIXELS_TO_METERS(width) / 2;
	float halfHeight = PIXELS_TO_METERS(height) / 2;

	b2PolygonShape shapeDef;
	if (!useRoundedBottom)
		shapeDef.SetAsBox(halfWidth, halfHeight);
	else
		CreatePolygonWithRoundedEdges(halfWidth, halfHeight, shapeDef);
		
	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shapeDef;
	fixtureDef.friction = friction;
	fixtureDef.restitution = restitution;
	fixtureDef.density = density;
	fixtureDef.isSensor = bSensorOnly;
	fixtureDef.filter.categoryBits = category;

	pkBody->CreateFixture(&fixtureDef);

	return pkBody;
}


b2Body* PhysicsManager::CreateStaticPhysicsBox( float x, float y, float width, float height, bool bSensorOnly, PhysicsCategory category)
{
	float density = 0.0f;
	float restitution = 0.0f;
	float friction = 2.0f;

	return CreatePhysicsBox(x,y,width,height, density, restitution, friction, false, bSensorOnly, false, category);
}

b2Body* PhysicsManager::CreateDynamicPhysicsBox( float x, float y, float width, float height, bool bDontAllowRotation, float fDensity, bool useRoundedBottom, PhysicsCategory category)
{
	float density = 1.0f;  // HACK THIS IN HERE. override what's passed in
	float restitution = 0.0f;
	float friction = 0.2f;

	b2Body* pkBody = CreatePhysicsBox(x,y,width,height, density, restitution, friction, bDontAllowRotation, false, useRoundedBottom, category);
	return pkBody;
}

void PhysicsManager::RemoveFromWorld( b2Body* pkBodyToRemove )
{
	assert(m_pkPhysicsWorld);
	m_pkPhysicsWorld->DestroyBody(pkBodyToRemove);
}

void PhysicsManager::ProcessCollisions()
{
	for (auto const& c : m_currentContacts) {
		ProcessCollision(&c.second);
	}
}

void PhysicsManager::ProcessCollision(const PhysicsContactInfo* pkb2Contact)
{
	b2WorldManifold worldManifold = pkb2Contact->worldManifold;

	// TODO: should just pass in PhysicsContact to OnCollide().  I'm just being lazy. Why am I even working on this project. Ok bye. -Dom2017

	if (pkb2Contact->objB)
		pkb2Contact->objB->OnCollide(pkb2Contact->objA, &worldManifold);

	worldManifold.normal = -worldManifold.normal;  // not sure if jank or OK.

	if (pkb2Contact->objA)
		pkb2Contact->objA->OnCollide(pkb2Contact->objB, &worldManifold);
}

void CreateCollisionInfo(const b2Contact* pkb2Contact, PhysicsContactInfo* contact_out) {
	const b2Fixture* fixtureA = pkb2Contact->GetFixtureA();
	const b2Fixture* fixtureB = pkb2Contact->GetFixtureB();

	contact_out->objA = (Object*)fixtureA->GetBody()->GetUserData();
	contact_out->objB = (Object*)fixtureB->GetBody()->GetUserData();

	pkb2Contact->GetWorldManifold(&contact_out->worldManifold);
}

void PhysicsContactListener::BeginContact(b2Contact* contact)
{
	PhysicsContactInfo contact_info;
	CreateCollisionInfo(contact, &contact_info);

	PHYSICS->m_currentContacts[contact] = contact_info;
}

void PhysicsContactListener::EndContact(b2Contact* contact)
{
	PHYSICS->m_currentContacts.erase(contact);
}