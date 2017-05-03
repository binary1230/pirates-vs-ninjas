#include "stdafx.h"
#include "physics.h"
#include "gameWorld.h"
#include "window.h"
#include "object.h"
#include "physicsDebugRenderer.h"
#include "gameState.h"

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

	m_blastRadius = 10;
	m_blastPower = 1000;

	memset(m_blastParticleBodies, 0, sizeof(m_blastParticleBodies));

	return true;
}

void PhysicsManager::Update()
{
	int32 velocityIterations = 6;  // TEMP TODO

	// Instruct the world to perform a single step of simulation. It is
	// generally best to keep the time step and iterations fixed.
	m_pkPhysicsWorld->Step(m_fPhysicsSimulatorTimeStep, velocityIterations, m_iPhysicsSimulatorIterations);

	UpdateExplosionParticles();

	// dispatch collision events now.
	ProcessCollisions();
}

void PhysicsManager::Shutdown()
{
	SAFE_DELETE(m_pkPhysicsWorld);
	m_currentContacts.clear();
}

void PhysicsManager::Draw()
{
	if (GAMESTATE->GetPropPhysicsDebugDraw()) {
		m_pkPhysicsWorld->DrawDebugData();
		DebugDrawParticles();
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

b2Body* PhysicsManager::CreatePhysicsBox( float x, float y, float width, float height, float density, float restitution, float friction, bool bDontAllowRotation /*= false */, bool bSensorOnly /*= false*/, bool useRoundedBottom )
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

	pkBody->CreateFixture(&fixtureDef);

	return pkBody;
}


b2Body* PhysicsManager::CreateStaticPhysicsBox( float x, float y, float width, float height, bool bSensorOnly )
{
	float density = 0.0f;
	float restitution = 0.0f;
	float friction = 2.0f;

	return CreatePhysicsBox(x,y,width,height, density, restitution, friction, false, bSensorOnly );
}

b2Body* PhysicsManager::CreateDynamicPhysicsBox( float x, float y, float width, float height, bool bDontAllowRotation, float fDensity, bool useRoundedBottom)
{
	float density = 1.0f;  // HACK THIS IN HERE. override what's passed in
	float restitution = 0.0f;
	float friction = 0.2f;

	b2Body* pkBody = CreatePhysicsBox(x,y,width,height, density, restitution, friction, bDontAllowRotation, false, useRoundedBottom);
	return pkBody;
}

void PhysicsManager::RemoveFromWorld( b2Body* pkBodyToRemove )
{
	assert(m_pkPhysicsWorld);
	m_pkPhysicsWorld->DestroyBody(pkBodyToRemove);
}

void PhysicsManager::ProcessCollisions()
{
	for (contact_iter c = m_currentContacts.begin(); c != m_currentContacts.end(); ++c) {
		ProcessCollision(&c->second);
	}
}

void PhysicsManager::ProcessCollision(PhysicsContactInfo* pkb2Contact)
{
	b2WorldManifold worldManifold = pkb2Contact->worldManifold;

	// TODO: should just pass in PhysicsContact to OnCollide().  I'm just being lazy. Why am I even working on this project. Ok bye. -Dom2017

	if (pkb2Contact->objB)
		pkb2Contact->objB->OnCollide(pkb2Contact->objA, &worldManifold);

	worldManifold.normal = -worldManifold.normal;  // not sure if jank or OK.

	if (pkb2Contact->objA)
		pkb2Contact->objA->OnCollide(pkb2Contact->objB, &worldManifold);
}

// _explosionType g_explosionType = ET_PROXIMITY;
int g_numRays = 32;
bool g_pauseAfterBlast = false;


/*
* ExplodeParticle() based on code from:
*
* Author: Chris Campbell - www.iforce2d.net
*
* Copyright (c) 2006-2011 Erin Catto http://www.box2d.org
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/
void PhysicsManager::ExplodeParticle(b2Vec2 center)
{
	center.x = PIXELS_TO_METERS(center.x);
	center.y = PIXELS_TO_METERS(center.y);

	//clear old particles
	for (int i = 0; i < MAX_BLAST_RAYS; i++) {
		if (m_blastParticleBodies[i]) {
			m_pkPhysicsWorld->DestroyBody(m_blastParticleBodies[i]);
			m_blastParticleBodies[i] = NULL;
		}
	}

	//clear previous positions
	for (int k = 0; k < m_previousParticlePositions.size(); k++)
		delete[] m_previousParticlePositions[k];

	m_previousParticlePositions.clear();

	for (int i = 0; i < g_numRays; i++) {
		float angle = DEG_TO_RAD((i / (float)g_numRays) * 360);
		b2Vec2 rayDir(sinf(angle), cosf(angle));

		b2BodyDef bd;
		bd.type = b2_dynamicBody;
		bd.fixedRotation = true;
		bd.bullet = true;
		bd.linearDamping = 10;
		bd.gravityScale = 0;
		bd.position = center;
		bd.linearVelocity = 0.125f * m_blastPower * rayDir;
		b2Body* body = m_pkPhysicsWorld->CreateBody(&bd);

		b2CircleShape circleShape;
		circleShape.m_radius = 0.05;

		b2FixtureDef fd;
		fd.shape = &circleShape;
		fd.density = 60 / (float)g_numRays;
		fd.friction = 0;
		fd.restitution = 0.99f;
		fd.filter.groupIndex = -1;
		body->CreateFixture(&fd);

		m_blastParticleBodies[i] = body;
	}
}

void PhysicsManager::DebugDrawParticles()
{
	//dashed lines to show where particles will go
	//display_raycast(false);

	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);

	//particle previous position trail
	glEnable(GL_BLEND);
	if (!m_previousParticlePositions.empty()) {
		for (int i = 0; i < MAX_BLAST_RAYS; i++) {
			if (m_blastParticleBodies[i]) {
				glBegin(GL_LINES);
				for (int k = 1; k < m_previousParticlePositions.size(); k++) {
					float alpha = (k - 1) / (float)m_previousParticlePositions.size();
					glColor4f(1, 1, 0, alpha);
					glVertex2fv((GLfloat*)&(m_previousParticlePositions[k - 1][i]));
					alpha = k / (float)m_previousParticlePositions.size();
					glColor4f(1, 1, 0, alpha);
					glVertex2fv((GLfloat*)&(m_previousParticlePositions[k][i]));
				}
				int k = m_previousParticlePositions.size() - 1;
				float alpha = k / (float)m_previousParticlePositions.size();
				glColor4f(1, 1, 0, alpha);
				glVertex2fv((GLfloat*)&(m_previousParticlePositions[k][i]));
				glColor4f(1, 1, 0, 1);
				b2Vec2 currentPos = m_blastParticleBodies[i]->GetPosition();
				glVertex2fv((GLfloat*)&currentPos);
				glEnd();
			}
		}
	}

	glEnable(GL_TEXTURE_2D);
}

//record positions of particles before stepping
void PhysicsManager::UpdateExplosionParticles() {
	if (m_previousParticlePositions.size() < 20) {
		b2Vec2* prevPositions = new b2Vec2[MAX_BLAST_RAYS];
		memset(prevPositions, 0, MAX_BLAST_RAYS * sizeof(b2Vec2));
		for (int i = 0; i < MAX_BLAST_RAYS; i++) {
			if (m_blastParticleBodies[i])
				prevPositions[i] = m_blastParticleBodies[i]->GetPosition();
		}
		m_previousParticlePositions.push_back(prevPositions);
	}
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