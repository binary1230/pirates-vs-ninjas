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

Effect* EffectsManager::FindEffectDefinition(const CString &effectName) {
  EffectDefMappingIter iter = effects.find(effectName);

  if (iter == effects.end())
    return NULL;

  return &(iter->second);
}

bool EffectsManager::AddEffectDefinition(	const CString &effectName,
                                        	XMLNode &xEffect) {
  if (effectName == "" || effectName.length() < 1)
    return false;

	Effect effect;

	effect.spawn_object_name = xEffect.getChildNode("spawn_object").getText();
	if (effect.spawn_object_name.length() < 1) {
		TRACE("ERROR: Effect object spawn name invalid in effect '%s'\n", 
										effectName.c_str());
		return false;
	}

	if (!(xEffect.nChildNode("camera_shake") == 1)) {
		effect.camera_shake = false;
		effect.camera_shake_duration = -1;
	} else {
		effect.camera_shake = true;
		if (!	xEffect.getChildNode("camera_shake")
					.getAttributeInt("duration", effect.camera_shake_duration) || 
					effect.camera_shake_duration < 0) {
			TRACE(	"ERROR: Effect camera shake duration "
												"invalid in effect '%s'\n", 
												effectName.c_str());
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
	} else {
		effect.display_time = -1;
	}
	
	if (xEffect.nChildNode("fade_time") == 1) {
		if (!xEffect.getChildNode("fade_time").getInt(effect.fadeout_time))
			return false;
	} else {
		effect.fadeout_time = -1;
	}

  effects[effectName] = effect;

  return true;
}

Object* EffectsManager::TriggerObject(	const Object* triggeringObject, 
																				CString effectName) {

	if (!triggeringObject) {
		TRACE("ERROR: Tried to trigger an effect with a NULL object!\n");
		return NULL;
	}
	
	Object* newObj = OBJECT_FACTORY->CreateObject(effectName);

	if (!newObj) {
		TRACE("ERROR: Unable to create effect object of type: '%s'\n", 
										effectName.c_str());
		return NULL;
	}

	newObj->SetXY( triggeringObject->GetXY() );
	newObj->SetLayer( triggeringObject->GetLayer() );
		
	WORLD->AddObject( newObj );

	return newObj;
}

Object* EffectsManager::TriggerEffect(	const Object* triggeringObject, 
																				CString effectName) {

	Effect* effect = FindEffectDefinition(effectName);

	if (!effect) {
		TRACE("EFFECTS: Can't find effect named '%s'\n", 
										effectName.c_str() );
		return NULL;
	}

	Object* obj = TriggerObject(triggeringObject, effect->spawn_object_name);

	if (!obj)
		return false;

	if (effect->center_x_on_target)
		obj->SetX(	int(triggeringObject->GetX() + 
								(float(triggeringObject->GetWidth()) / 2.0f) -
								(float(obj->GetWidth()) / 2.0f) ));

	if (effect->center_y_on_target)
		obj->SetY(	int(triggeringObject->GetY() + 
								(float(triggeringObject->GetHeight()) / 2.0f) -
								(float(obj->GetHeight()) / 2.0f) ));

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
	static const char* parent_include = "The Toplevel effects XML file";
  static int recurse_level = 0;

	XMLNode xEffect, xEffectDefFile;
	CString effectName, file;

	max = xEffects.nChildNode("effect");
	for (i = iterator = 0; i < max; ++i) {
		xEffect = xEffects.getChildNode("effect", &iterator);
		effectName = xEffect.getAttribute("name");
		
		if (!FindEffectDefinition(effectName)) {
			if (!AddEffectDefinition(effectName, xEffect)) {
				TRACE("ERROR: Failed to add effect definition '%s'\n", 
												effectName.c_str());
				return false;
			}
		} else {
      TRACE("EffectsManager: WARNING: Duplicate effect "
	                    "definition found for effect name: '%s', ignoring.\n",
                      effectName.c_str());
		}
	}

  // 2) handle <include_xml_file> tags 
  max = xEffects.nChildNode("include_xml_file");

  for (i = iterator = 0; i < max; ++i) {

    // get the filename
    file = xEffects.getChildNode("include_xml_file", &iterator).getText();

    CString fileNew = ASSETMANAGER->GetPathOf(file);

    if (!fileNew.size()) {
      TRACE("EffectsManager: ERROR: Can't open "
                      "requested XML file for inclusion: '%s'\n",
                      file.c_str() );
      return false;
    }

    // this method is recursive, let's make sure
    // we don't fall into any infinite loops.
    if (++recurse_level > 99) {
      TRACE(  "ERROR: Infinite loop while reading effect\n"
                        "       definitions!!  Make sure that that\n"
                        "       '%s' does not include itself!\n",
                        parent_include);

      return false;
    }

    parent_include = fileNew.c_str();

    xEffectDefFile = XMLNode::openFileHelper( fileNew.c_str(),
                                              "effects");

    // recursively call ourself to handle this
    if (!LoadEffectsFromXML(xEffectDefFile))
      return false;

    --recurse_level;
  }

	return true;
}

void EffectsManager::Shutdown() {
	effects.clear();
}
