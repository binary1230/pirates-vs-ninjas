#pragma once

#include "globals.h"

#define MAX_BLAST_RAYS 128

class PhysicsExplosion {
	protected:
		float _blastRadius;
		float _blastPower;

		int _timeRemaining;
		bool _isDead;

		b2Body* _blastParticleBodies[MAX_BLAST_RAYS];
		std::vector<b2Vec2*> _previousParticlePositions;

	public:
		void Update();
		void UpdatePreviousParticlePositions();
		void DebugDraw();
		void ExplodeAt(b2Vec2 center);

		bool IsDead();
		void Disable();

		PhysicsExplosion();
		~PhysicsExplosion();
};