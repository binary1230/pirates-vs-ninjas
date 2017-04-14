#include "stdafx.h"
#include "effectsManager.h"
#include "globals.h"
#include "gameWorld.h"
#include "object.h"
#include "objectFactory.h"
#include "assetManager.h"

DECLARE_SINGLETON(EffectsManager);

EffectsManager::EffectsManager() {}
EffectsManager::~EffectsManager() {
	Shutdown();
}

bool EffectsManager::Init() {
	Shutdown();
	return true;
}

Effect* EffectsManager::FindEffectDefinition(const std::string &effectName) {
	EffectDefMappingIter iter = effects.find(effectName);

	if (iter == effects.end())
		return NULL;

	return &(iter->second);
}

bool EffectsManager::AddEffectDefinition(const std::string &effectName,
	XMLNode &xEffect) {
	if (effectName == "" || effectName.length() < 1)
		return false;

	Effect effect;

	effect.spawn_object_name = xEffect.getChildNode("spawn_object").getText();
	if (effect.spawn_object_name.length() < 1) {
		TRACE("ERROR: Effect object spawn name invalid in effect '%s'\n",
			effectName);
		return false;
	}

	if (!(xEffect.nChildNode("camera_shake") == 1)) {
		effect.camera_shake = false;
		effect.camera_shake_duration = -1;
	}
	else {
		effect.camera_shake = true;
		if (!xEffect.getChildNode("camera_shake")
			.getAttributeInt("duration", effect.camera_shake_duration) ||
			effect.camera_shake_duration < 0) {
			TRACE("ERROR: Effect camera shake duration "
				"invalid in effect '%s'\n",
				effectName);
			return false;
		}
	}

	if (xEffect.nChildNode("center_x_on_target") == 1)
		effect.center_x_on_target = true;
	else
		effect.center_x_on_target = false;

	if (xEffect.nChildNode("center_y_on_target") == 1)
		effect.center_y_on_target = true;
	else
		effect.center_y_on_target = false;

	if (xEffect.nChildNode("display_time") == 1) {
		if (!xEffect.getChildNode("display_time").getInt(effect.display_time))
			return false;
	}
	else {
		effect.display_time = -1;
	}

	if (xEffect.nChildNode("fade_time") == 1) {
		if (!xEffect.getChildNode("fade_time").getInt(effect.fadeout_time))
			return false;
	}
	else {
		effect.fadeout_time = -1;
	}

	effects[effectName] = effect;

	return true;
}

Object* EffectsManager::TriggerObject(const Object* triggeringObject,
	std::string effectName) {
	if (!triggeringObject) {
		TRACE("ERROR: Tried to trigger an effect with a NULL object!\n");
		return NULL;
	}

	Object* newObj = NULL;

#ifdef USE_OLD_LOADING_SYSTEM
	// temporarily disabled, nothing really to do with loading system
	newObj = OBJECT_FACTORY->CreateObject(effectName);

	if (!newObj) {
		TRACE("ERROR: Unable to create effect object of type: '%s'\n",
			effectName);
		return NULL;
	}

	newObj->SetXY(triggeringObject->GetXY());
	newObj->SetLayer(triggeringObject->GetLayer());

	const bool addImmediately = true;	// might be slightly risk to do this, 
										// but needed for some physics stuff to work right away
	WORLD->AddObject(newObj, addImmediately);
#endif USE_OLD_LOADING_SYSTEM

	return newObj;
}

Object* EffectsManager::TriggerEffect(const Object* triggeringObject,
	std::string effectName)
{
	Effect* effect = FindEffectDefinition(effectName);

	if (!effect) {
		TRACE("EFFECTS: Can't find effect named '%s'\n",
			effectName);
		return NULL;
	}

	Object* obj = TriggerObject(triggeringObject, effect->spawn_object_name);

	if (!obj)
		return false;

	if (effect->center_x_on_target)
		obj->SetX(int(triggeringObject->GetX() +
		(float(triggeringObject->GetWidth()) / 2.0f) -
			(float(obj->GetWidth()) / 2.0f)));

	if (effect->center_y_on_target)
		obj->SetY(int(triggeringObject->GetY() +
		(float(triggeringObject->GetHeight()) / 2.0f) -
			(float(obj->GetHeight()) / 2.0f)));

	if (effect->camera_shake)
		WORLD->SetCameraShake(true, effect->camera_shake_duration);

	if (effect->display_time != -1)
		obj->SetDisplayTime(effect->display_time);

	if (effect->fadeout_time != -1)
		obj->FadeOut(effect->fadeout_time);

	return obj;
}

bool EffectsManager::LoadEffectsFromXML(XMLNode &xEffects) {
	int i, max, iterator;

	XMLNode xEffect;
	std::string effectName, file;

	max = xEffects.nChildNode("effect");
	for (i = iterator = 0; i < max; ++i) {
		xEffect = xEffects.getChildNode("effect", &iterator);
		effectName = xEffect.getAttribute("name");

		if (!FindEffectDefinition(effectName)) {
			if (!AddEffectDefinition(effectName, xEffect)) {
				TRACE("ERROR: Failed to add effect definition '%s'\n", effectName);
				return false;
			}
		}
		else {
			TRACE("EffectsManager: WARNING: Duplicate effect definition found for effect name: '%s', ignoring.\n", effectName);
		}
	}

	return true;
}

bool EffectsManager::LoadEffectsFromIncludedXml(const std::string filename) {
	std::string full_path = ASSETMANAGER->GetPathOf(filename.c_str());

	if (!full_path.length()) {
		TRACE("EffectsManager: ERROR: Can't open requested XML file for inclusion: '%s'\n", filename);
		return false;
	}

	XMLNode xEffectDefFile = XMLNode::openFileHelper(full_path.c_str(), "effects");

	return LoadEffectsFromXML(xEffectDefFile);
}

void EffectsManager::Shutdown() {
	effects.clear();
}
