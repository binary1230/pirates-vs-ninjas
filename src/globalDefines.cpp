#include "stdafx.h"
#include "globalDefines.h"

DECLARE_SINGLETON(GlobalDefines)

GlobalDefines::GlobalDefines() {
	initialized = false; 
	Shutdown();
}

GlobalDefines::~GlobalDefines() {
	Shutdown(); 
}

bool GlobalDefines::Init(XMLNode &_xParams) {
	assert(!initialized);

	xParams = _xParams;
	initialized = true;

	return initialized;
}

void GlobalDefines::Shutdown() {
	if (!initialized)
		return;

	xParams = XMLNode::emptyXMLNode;

	initialized = false;
}

bool GlobalDefines::Value(char* varName, bool &out) {
	int temp_out;
	bool ret_val = Value(varName, temp_out);
	
	if (temp_out)
		out = true;
	else
		out = false;

	assert(ret_val);
	return ret_val;
}

bool GlobalDefines::Value(char* varName, int &out) {
	if (!xParams.nChildNode(varName)) {
		TRACE("ERROR: Can't find global XML variable named '%s'\n", 
										varName);
		return false;
	}
	
	return xParams.getChildNode(varName).getInt(out);
}

bool GlobalDefines::Value(char* varName, float &out) {
	if (!xParams.nChildNode(varName)) {
		TRACE("ERROR: Can't find global XML variable named '%s'\n", 
										varName);
		return false;
	}

	return xParams.getChildNode(varName).getFloat(out);
}

bool GlobalDefines::Value(char* varName, CString &out) {
	if (!xParams.nChildNode(varName)) {
		TRACE("ERROR: Can't find global XML variable named '%s'\n", 
										varName);
		return false;
	}

	out = xParams.getChildNode(varName).getText();
	return true;
}
