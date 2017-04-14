#ifndef EFFECTSMANAGER_H
#define EFFECTSMANAGER_H

// Game effects manager:
// Handles effects like shields, dust, explosions, footprints, etc.
//
// Usually, you get here from the animation frame handler code

#include "globals.h"

class Object;

struct Effect {
	std::string spawn_object_name;

	bool camera_shake;
	int  camera_shake_duration;
	bool center_x_on_target;
	bool center_y_on_target;

	int display_time;
	int fadeout_time;
};

typedef map<const std::string, Effect> EffectDefMapping;
typedef map<const std::string, Effect>::iterator EffectDefMappingIter;

class EffectsManager {
		DECLARE_SINGLETON_CLASS(EffectsManager)

		protected:
			EffectDefMapping effects;

		public:
			bool Init();
			void Shutdown();

			bool AddEffectDefinition(const std::string &effectName, XMLNode &xEffect);

			Effect* FindEffectDefinition(const std::string &effectName);

			bool LoadEffectsFromXML(XMLNode &xEffects);

			bool LoadEffectsFromIncludedXml(const std::string filename);

			// Trigger an object at a given object, insert it into the simulation,
			// and return a pointer to the newly inserted object
			Object* TriggerObject(const Object* triggeringObject, std::string objectName);

			// The usual case.
			// Trigger an effect (as specified in an effect XML file)
			// at the current object
			Object* TriggerEffect(const Object* triggeringObject, std::string effectName);

			virtual ~EffectsManager();
};

#define EFFECTS EffectsManager::GetInstance()

#endif // EFFECTSMANAGER_H
