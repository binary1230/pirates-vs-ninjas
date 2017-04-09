#include "stdafx.h"
#include "objectPlayerHuman.h"
#include "input.h"

HumanPlayerObject::HumanPlayerObject() {
	
}

HumanPlayerObject::~HumanPlayerObject() {

}

bool HumanPlayerObject::GetInput(uint key, uint controller_num) const 
{
	return INPUT->Key(key, controller_num);
}

BOOST_CLASS_EXPORT_GUID(HumanPlayerObject, "HumanPlayerObject")