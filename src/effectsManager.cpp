#include "effectsManager.h"
#include "globals.h"
#include "physSimulation.h"
#include "object.h"
#include "objectFactory.h"

DECLARE_SINGLETON(EffectsManager);

EffectsManager::EffectsManager() {}
EffectsManager::~EffectsManager() {
	Shutdown();
}

bool EffectsManager::Init(PhysSimulation* p) {
	simulation = p;
	return true;	
}

void EffectsManager::Shutdown() {
	simulation = NULL;
}

Object* EffectsManager::Trigger(	const Object* triggeringObject, 
																	CString effectName) {

	if (!triggeringObject) {
		fprintf(stderr, "ERROR: Tried to trigger an effect with a NULL object!\n");
		return NULL;
	}
	
	Object* newObj = OBJECT_FACTORY->CreateObject(effectName);

	if (!newObj) {
		fprintf(stderr, "ERROR: Unable to create effect object of type: '%s'\n", 
										effectName.c_str());
		return NULL;
	}

	newObj->SetXY( triggeringObject->GetXY() );
	newObj->SetLayer( triggeringObject->GetLayer() );
		
	simulation->AddObject( newObj );

	return newObj;
}
