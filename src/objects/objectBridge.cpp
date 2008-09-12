#include "stdafx.h"
#include "objectBridge.h"

bool BridgeObject::Init()
{
	if (!Object::BaseInit())
		return false;

	return true;
}

void BridgeObject::Shutdown()
{
	
}

BridgeObject::BridgeObject()
{

}

BridgeObject::~BridgeObject()
{

}

void BridgeObject::Draw()
{

}

void BridgeObject::Update()
{

}