#ifndef EFFECTSMANAGER_H
#define EFFECTSMANAGER_H

// Game effects manager:
// Handles effects like shields, dust, explosions, footprints, etc.
//
// Usually, you get here from the animation frame handler code

#include "stdafx.h"
#include "singleton.h"

class Object;
class PhysSimulation;

class EffectsManager {
		DECLARE_SINGLETON_CLASS(EffectsManager)

		protected:

			// urgh, DAMN IT make this a singleton jeez.
			PhysSimulation* simulation;	

		public:

			bool Init(PhysSimulation* p);
			void Shutdown();

			// Trigger an effect at a given object, insert it into the simulation,
			// and return a pointer to the newly inserted object
			Object* Trigger(const Object* triggeringObject, CString effectName);

			virtual ~EffectsManager();
};

#define EFFECTS EffectsManager::GetInstance()

#endif // EFFECTSMANAGER_H
