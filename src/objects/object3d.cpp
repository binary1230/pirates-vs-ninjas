#include "object3d.h"

#include "globals.h"
#include "gameState.h"
#include "Model_3DS.h"

void ModelObject::Shutdown() {
	delete model;
	BaseShutdown();
}

void ModelObject::Update() {
	BaseUpdate();
}

bool ModelObject::Init(PhysSimulation *p) {
	simulation = p;
	return BaseInit();
}

void ModelObject::Draw() {
	model->Draw();
}

ModelObject::ModelObject() {}
ModelObject::~ModelObject() {}
