#ifndef OBJECT_FACTORY_H
#define OBJECT_FACTORY_H

#include "stdafx.h"

class ObjectFactory;
class AssetManager;

// All must declare objectFactory as friend classes
class Object;
class ObjectPlayer;
class ObjectRadiusBlock;
class ObjectCollectable;
class ObjectController;
class ObjectBackground;
class ObjectStatic;
class ObjectSpring;
class PhysSimulation;

#include "singleton.h"
#include "animation.h"
#include "objectIDs.h"

//! Maps an object definition name to an XMLNode 
//! (e.g. maps "bad_guy_1" to its corresponding XML data)
//! only used for parsing XML
typedef map<const CString, XMLNode> ObjectDefMapping;
typedef map<const CString, XMLNode>::iterator ObjectDefMappingIter;

//! A class which creates Object classes from integer ID's
class ObjectFactory {

	DECLARE_SINGLETON_CLASS(ObjectFactory)

	protected:
		Object* NewPlayerObject(XMLNode &xDef, XMLNode &xObj);
		Object* NewRadiusBlockObject(XMLNode &xDef, XMLNode &xObj);
		Object* NewCollectableObject(XMLNode &xDef, XMLNode &xObj);
		Object* NewControllerObject(XMLNode &xDef, XMLNode &xObj);
		Object* NewBackgroundObject(XMLNode &xDef, XMLNode &xObj);
		Object* NewStaticObject(XMLNode &xDef, XMLNode &xObj);
		Object* NewSpringObject(XMLNode &xDef, XMLNode &xObj);
		Object* NewFanObject(XMLNode &xDef, XMLNode &xObj);
		Object* NewDoorObject(XMLNode &xDef, XMLNode &xObj);
		
		bool LoadObjectSounds(Object* obj, XMLNode &xDef);
		bool LoadObjectProperties(Object* obj, XMLNode &xDef);
		bool LoadObjectAnimations(Object* obj, XMLNode &xDef, 
															AnimationMapping *animation_lookup = NULL);

		// XXX remove once PhysSimulation is a singleton
		PhysSimulation *physSimulation;

		//! Holds object definitions
		//! e.g. the definition of e.g. a "player" object 
		//! is which frames it has, movement speed, etc
		ObjectDefMapping objectDefs;

	public:
		int Init();
		void Shutdown();
		
		inline void SetPhysSimulation(PhysSimulation* p) {physSimulation = p;};
		inline PhysSimulation* GetPhysSimulation() {return physSimulation;};

		// Create an object from an XML node
		Object* CreateObjectFromXML(	XMLNode &xObjectDef, XMLNode &xObject);

		// Create an object from an ObjectID
		Object* CreateObject(OBJECTID object_id);

		bool AddObjectDefinition(	const CString &objDefName, 
															const XMLNode &xObjectDef);

		// Return the XML associated with an object definition
		XMLNode* FindObjectDefinition(const CString &objDefName);

		~ObjectFactory();
};

#define OBJECT_FACTORY ObjectFactory::GetInstance()

#endif // OBJECT_FACTORY_H
