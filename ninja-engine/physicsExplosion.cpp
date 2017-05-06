
/*
* Particle explosion code mostly based on:
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

#include "stdafx.h"

#include "globals.h"
#include "physics.h"
#include "physicsExplosion.h"

int g_numRays = 32;

void PhysicsExplosion::ExplodeAt(b2Vec2 center)
{
	assert(PHYSICS);
	assert(_isDead);
	assert(_timeRemaining == 0);

	_timeRemaining = 30;
	_isDead = false;

	center.x = PIXELS_TO_METERS(center.x);
	center.y = PIXELS_TO_METERS(center.y);

	//clear old particles
	for (int i = 0; i < MAX_BLAST_RAYS; i++) {
		if (_blastParticleBodies[i]) {
			PHYSICS->GetPhysicsWorld()->DestroyBody(_blastParticleBodies[i]);
			_blastParticleBodies[i] = NULL;
		}
	}

	//clear previous positions
	for (int k = 0; k < _previousParticlePositions.size(); k++)
		delete[] _previousParticlePositions[k];

	_previousParticlePositions.clear();

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
		bd.linearVelocity = 0.125f * _blastPower * rayDir;
		b2Body* body = PHYSICS->GetPhysicsWorld()->CreateBody(&bd);
		body->SetUserData(nullptr);

		b2CircleShape circleShape;
		circleShape.m_radius = 0.05;

		b2FixtureDef fd;
		fd.shape = &circleShape;
		fd.density = 60 / (float)g_numRays;
		fd.friction = 0;
		fd.restitution = 0.99f;
		fd.filter.groupIndex = -1;
		body->CreateFixture(&fd);

		_blastParticleBodies[i] = body;
	}
}

void PhysicsExplosion::DebugDraw()
{
	//dashed lines to show where particles will go
	//display_raycast(false);

	glLoadIdentity();
	glDisable(GL_TEXTURE_2D);

	GLfloat* v;

	//particle previous position trail
	glEnable(GL_BLEND);
	if (!_previousParticlePositions.empty()) {
		for (int i = 0; i < MAX_BLAST_RAYS; i++) {
			if (_blastParticleBodies[i]) {
				glBegin(GL_LINES);
				for (int k = 1; k < _previousParticlePositions.size(); k++) {
					
					float alpha = (k - 1) / (float)_previousParticlePositions.size();
					glColor4f(1, 1, 0, alpha);
					v = (GLfloat*)&(_previousParticlePositions[k - 1][i]);
					PhysicsDebugRenderer::SubmitVertex(v[0], v[1]);

					alpha = k / (float)_previousParticlePositions.size();
					glColor4f(1, 1, 0, alpha);
					v = (GLfloat*)&(_previousParticlePositions[k][i]);
					PhysicsDebugRenderer::SubmitVertex(v[0], v[1]);
				}
				int k = _previousParticlePositions.size() - 1;
				float alpha = k / (float)_previousParticlePositions.size();
				glColor4f(1, 1, 0, alpha);
				v = (GLfloat*)&(_previousParticlePositions[k][i]);
				PhysicsDebugRenderer::SubmitVertex(v[0], v[1]);

				glColor4f(1, 1, 0, 1);
				b2Vec2 currentPos = _blastParticleBodies[i]->GetPosition();
				v = (GLfloat*)&currentPos;
				PhysicsDebugRenderer::SubmitVertex(v[0], v[1]);

				glEnd();
			}
		}
	}

	glEnable(GL_TEXTURE_2D);
}

bool PhysicsExplosion::IsDead()
{
	return _isDead;
}

void PhysicsExplosion::Disable() {
	for (int i = 0; i < MAX_BLAST_RAYS; i++) {
		if (_blastParticleBodies[i]) {
			_blastParticleBodies[i]->SetActive(false);
		}
	}
}

//record positions of particles before stepping
void PhysicsExplosion::Update() {
	if (_timeRemaining > 0)
		_timeRemaining--;
	
	if (_timeRemaining == 0 && !_isDead) {
		Disable();
		_isDead = true;
	}

	if (_isDead)
		return;

	UpdatePreviousParticlePositions();
}

void PhysicsExplosion::UpdatePreviousParticlePositions()
{
	if (_previousParticlePositions.size() < 20) {
		b2Vec2* prevPositions = new b2Vec2[MAX_BLAST_RAYS];
		memset(prevPositions, 0, MAX_BLAST_RAYS * sizeof(b2Vec2));
		for (int i = 0; i < MAX_BLAST_RAYS; i++) {
			if (_blastParticleBodies[i])
				prevPositions[i] = _blastParticleBodies[i]->GetPosition();
		}
		_previousParticlePositions.push_back(prevPositions);
	}
}

PhysicsExplosion::PhysicsExplosion() {
	_blastRadius = 10;
	_blastPower = 1000;

	_timeRemaining = 0;
	_isDead = true;

	memset(_blastParticleBodies, 0, sizeof(_blastParticleBodies));
}

PhysicsExplosion::~PhysicsExplosion() {
}
