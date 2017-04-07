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

	b2Vec2 gravity(0.0f, -15.0f);

	assert(m_pkPhysicsWorld == NULL);
	m_pkPhysicsWorld = new b2World(gravity);

	// NOTE FROM DOM: If you are getting errors here, it's because I hacked the source
	// of Box2D to 1) make SetDebugDraw() public, and 2) remove the call from b2World::Step()
	// TEMPHACK 2017 // m_kPhysicsDebugRenderer.SetFlags(PhysicsDebugRenderer::e_shapeBit);
	// TEMPHACK 2017 // m_pkPhysicsWorld->SetDebugDraw(&m_kPhysicsDebugRenderer);

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

	int32 velocityIterations = 6;  // TEMP TODO

	// Instruct the world to perform a single step of simulation. It is
	// generally best to keep the time step and iterations fixed.
	m_pkPhysicsWorld->Step(m_fPhysicsSimulatorTimeStep, velocityIterations, m_iPhysicsSimulatorIterations);

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

b2Body* PhysicsManager::CreatePhysicsBox( float x, float y, float width, float height, float density, float restitution, float friction, bool bDontAllowRotation /*= false */, bool bSensorOnly /*= false*/ )
{
	b2BodyDef bodyDef;

	assert(m_pkPhysicsWorld);
	assert(width > 0.0f);
	assert(height > 0.0f);

	float halfWidth = PIXELS_TO_METERS(width) / 2;
	float halfHeight = PIXELS_TO_METERS(height) / 2;

	bodyDef.position.Set(PIXELS_TO_METERS(x) + halfWidth, PIXELS_TO_METERS(y) + halfHeight);
	bodyDef.fixedRotation = bDontAllowRotation;
	//bodyDef.linearDamping = 0.4f;

	if (density > 0.0f)
		bodyDef.type = b2_dynamicBody;

	b2Body* pkBody = m_pkPhysicsWorld->CreateBody(&bodyDef);
	assert(pkBody);

	b2PolygonShape shapeDef;
	shapeDef.SetAsBox(halfWidth, halfHeight);

	b2FixtureDef fixtureDef;
	fixtureDef.shape = &shapeDef;
	fixtureDef.friction = friction;
	fixtureDef.restitution = restitution;
	fixtureDef.density = density;
	fixtureDef.isSensor = bSensorOnly;

	pkBody->CreateFixture(&fixtureDef);

	return pkBody;
}


b2Body* PhysicsManager::CreateStaticPhysicsBox( float x, float y, float width, float height, bool bSensorOnly )
{
	return CreatePhysicsBox(x,y,width,height, 0.0f, 0.0f, 0.2f, false, bSensorOnly );
}

b2Body* PhysicsManager::CreateDynamicPhysicsBox( float x, float y, float width, float height, bool bDontAllowRotation, float fDensity )
{
	// TODO: Don't hardcode these numbers.
	b2Body* pkBody = CreatePhysicsBox(x,y,width,height, fDensity, 0.0f, 0.2f, bDontAllowRotation);
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

void PhysicsManager::ProcessCollision(PhysicsContact* pkb2Contact)
{
	b2WorldManifold worldManifold = pkb2Contact->worldManifold;

	// TODO: should just pass in PhysicsContact to OnCollide().  I'm just being lazy. Why am I even working on this project. Ok bye. -Dom2017

	pkb2Contact->objB->OnCollide(pkb2Contact->objA, &worldManifold);
	worldManifold.normal = -worldManifold.normal;  // not sure if jank or OK.
	pkb2Contact->objA->OnCollide(pkb2Contact->objB, &worldManifold);
}

void PhysicsManager::Reportb2Contact(const b2Contact* pkb2Contact)
{
	PhysicsContact contact;

	const b2Fixture* fixtureA = pkb2Contact->GetFixtureA();
	const b2Fixture* fixtureB = pkb2Contact->GetFixtureB();

	contact.objA = (Object*)fixtureA->GetBody()->GetUserData();
	contact.objB = (Object*)fixtureB->GetBody()->GetUserData();

	pkb2Contact->GetWorldManifold(&contact.worldManifold);

	m_kContacts.push_back(contact);
}

void PhysicsContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{
	// this is the one that is triggered each frame if objects are touching

	// TRACE("%i: Add\n", i);
	PHYSICS->Reportb2Contact(contact);
}

void PhysicsContactListener::BeginContact(b2Contact* contact)
{
	// we're only going to use this to report sensors 
	// (i.e. when there should not be a collision, just a notification)

	const b2Fixture* fixtureA = contact->GetFixtureA();
	const b2Fixture* fixtureB = contact->GetFixtureB();

	Object* objA = (Object*)fixtureA->GetBody()->GetUserData();
	Object* objB = (Object*)fixtureB->GetBody()->GetUserData();

	b2WorldManifold manifold;
	contact->GetWorldManifold(&manifold);

	if (fixtureA->IsSensor() || fixtureB->IsSensor()) {
		objA->OnCollide(objB, &manifold);
		manifold.normal = -manifold.normal; // not sure if jank or OK.
		objB->OnCollide(objA, &manifold);
	}
}
