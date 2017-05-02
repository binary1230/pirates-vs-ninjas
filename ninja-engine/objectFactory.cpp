#include "stdafx.h"
#include "objectFactory.h"

#include "globals.h"
#include "gameState.h"
#include "sprite.h"
#include "object.h"
#include "objectIDs.h"


#include "objectPlayer.h"
#include "objectBounce.h"
#include "objectBackground.h"
#include "objectController.h"
#include "objectStatic.h"
#include "objectSpring.h"
#include "objectCollectable.h"
#include "objectFan.h"
#include "objectDoor.h"
#include "objectEnemy.h"
#include "objectText.h"
#include "objectCutBars.h"


#include "assetManager.h"
#include "animations.h"
#include "gameSound.h"
#include "gameWorld.h"

DECLARE_SINGLETON(ObjectFactory)

void RegisterObjectPrototypes() {
	// this needs to be called very early in the startup process

	MAKE_PROTOTYPE(ObjectPlayer);
	MAKE_PROTOTYPE(ObjectBounce);
	MAKE_PROTOTYPE(ObjectBackground);
	MAKE_PROTOTYPE(ObjectController);
	MAKE_PROTOTYPE(ObjectStatic);
	MAKE_PROTOTYPE(ObjectSpring);
	MAKE_PROTOTYPE(ObjectCollectable);
	MAKE_PROTOTYPE(ObjectFan);
	MAKE_PROTOTYPE(ObjectDoor);
	MAKE_PROTOTYPE(ObjectEnemy);
	MAKE_PROTOTYPE(ObjectText);
	MAKE_PROTOTYPE(ObjectCutBars);

	// these are here for backwards compatibility.  we should update the XML
	// data in order to use the new names above, and then remove this section
	MAKE_PROTOTYPE_ALIAS(ObjectPlayer, "Player");
	MAKE_PROTOTYPE_ALIAS(ObjectBackground, "Background");
	MAKE_PROTOTYPE_ALIAS(ObjectController, "ControllerDisplay");
	MAKE_PROTOTYPE_ALIAS(ObjectStatic, "Static");
	MAKE_PROTOTYPE_ALIAS(ObjectEnemy, "Enemy");
	MAKE_PROTOTYPE_ALIAS(ObjectFan, "Fan");
	MAKE_PROTOTYPE_ALIAS(ObjectDoor, "Door");
	MAKE_PROTOTYPE_ALIAS(ObjectSpring, "Spring");
	MAKE_PROTOTYPE_ALIAS(ObjectCollectable, "Collectable");
	MAKE_PROTOTYPE_ALIAS(ObjectText, "TextOverlay");
	MAKE_PROTOTYPE_ALIAS(ObjectCutBars, "CutBars");
	MAKE_PROTOTYPE_ALIAS(ObjectBounce, "Bounce");
}

bool ObjectFactory::AddObjectDefinition(const std::string &objDefName, const XMLNode &xObjectDef) {
	if (objDefName == "" || objDefName.length() < 1)
		return false;

	objectDefs[objDefName] = xObjectDef;

	return true;
}

XMLNode* ObjectFactory::FindObjectDefinition(const std::string &objDefName) {
	ObjectDefMappingIter iter = objectDefs.find(objDefName);

	if (iter == objectDefs.end())
		return NULL;
	
	return &(iter->second);
}

int ObjectFactory::GetObjectDefinitionCount() const {
	return objectDefs.size();
}

const std::string& ObjectFactory::GetObjectDefinition(int iIndex) const {
	assert(iIndex >= 0 && iIndex < (int)objectDefs.size());
	ObjectDefMappingConstIter iter = objectDefs.begin();
	
	for (int i = 0; i != iIndex; ++i)
		++iter;

	assert(iter != objectDefs.end());

	return iter->first;
}
	
//! Recursively loads Object Definitions from XML, 
//! puts them into an ObjectMapping
bool ObjectFactory::LoadObjectDefsFromXML(XMLNode &xObjDefs) {

	// Object definitions can take 2 forms in the XML file
	// 1) an <objectDef> tag
	// 2) an <include_xml_file> tag which we then open and get an <objectDef>
	
	int i, max, iterator;
	static const char* parent_include = "The Toplevel XML file";
	static int recurse_level = 0;

	XMLNode xObjectDef, xObjectDefFile;
	std::string objName, file;
	
	// 1) handle <objectDef> tags
	max = xObjDefs.nChildNode("objectDef");
	for (i = iterator = 0; i < max; i++) {
		xObjectDef = xObjDefs.getChildNode("objectDef", &iterator);
		objName = xObjectDef.getAttribute("name");
		
		if (!FindObjectDefinition(objName)) {
			AddObjectDefinition(objName, xObjectDef);
		} else {
			TRACE("ObjectFactory: WARNING: Duplicate object "
											"definitions found for object name: '%s', ignoring.\n",
											objName);
		}
	}

	// 2) handle <include_xml_file> tags 
	max = xObjDefs.nChildNode("include_xml_file");
	
	for (i = iterator = 0; i < max; i++) {
		file = xObjDefs.getChildNode("include_xml_file", &iterator).getText();
		if (!LoadObjectDefsFromIncludeXML(file)) {
			return false;
		}
	}

	return true;
}

bool ObjectFactory::LoadObjectDefsFromIncludeXML(std::string file) {
	std::string full_path = ASSETMANAGER->GetPathOf(file.c_str());

	if (!full_path.length()) {
		TRACE("ObjectFactory: ERROR: Can't open requested XML file for inclusion: '%s'\n", file);
		return false;
	}

	XMLNode xObjectDefFile = XMLNode::openFileHelper(full_path.c_str(), "objectDefinitions");

	return LoadObjectDefsFromXML(xObjectDefFile);
}


// Get the object ID from an XML object definition
std::string ObjectFactory::GetClassNameFromXML(XMLNode &xObjectDef) {
	return xObjectDef.getAttribute("type");
}

int ObjectFactory::Init() {
	objectDefs.clear();
	return 0;
}

void ObjectFactory::Shutdown() {
	objectDefs.clear();
}

ObjectFactory::ObjectFactory() {}
ObjectFactory::~ObjectFactory() {}

Object* ObjectFactory::CreateObject(std::string objDefName) {
	XMLNode* xObjectDef = FindObjectDefinition(objDefName);

	if (!xObjectDef) {
		TRACE("ObjectFactory: Unable to instantiate object definition: '%s'\n", objDefName);
		return NULL;
	}

	Object* pkObject = CreateObjectFromXML(*xObjectDef);
	if (!pkObject)
		return false;

	pkObject->SetObjectDefName(objDefName.c_str());
	return pkObject;
}

// Creates an object from an XML definition
// in: xObjectDef - XML representation of an object's definition
// in: xObject - optional: XML representation of additional object paramaters.
// returns: newly create Object*, or NULL if it failed
Object* ObjectFactory::CreateObjectFromXML(XMLNode &xDef)
{
	assert(WORLD != NULL);

	Object* obj = NULL;
	string className = GetClassNameFromXML(xDef);

	obj = Object::CreateObject(className);

	if (!obj->LoadFromObjectDef(xDef))
		return NULL;

	return obj;
}