#ifndef OBJECT_FACTORY_H
#define OBJECT_FACTORY_H

class Object;
class ObjectFactory;
class AssetManager;

#include "animation.h"
#include "objectIDs.h"

//! Maps an object definition name to an XMLNode 
//! (e.g. maps "bad_guy_1" to its corresponding XML data)
//! only used for parsing XML
typedef map<const std::string, XMLNode> ObjectDefMapping;
typedef map<const std::string, XMLNode>::iterator ObjectDefMappingIter;
typedef map<const std::string, XMLNode>::const_iterator ObjectDefMappingConstIter;

//! A class which creates Object classes from integer ID's
class ObjectFactory {

	DECLARE_SINGLETON_CLASS(ObjectFactory)

	protected:
		//! Holds object definitions
		//! e.g. the definition of e.g. a "player" object 
		//! is which frames it has, movement speed, etc
		ObjectDefMapping objectDefs;

		std::string GetClassNameFromXML(XMLNode &xObjectDef);

	public:
		int Init();
		void Shutdown();
		
		// Create an object from an XML node
		Object* CreateObjectFromXML(XMLNode &xObjectDef, XMLNode *xObject);

		// Create an object from a string
		Object* CreateObject(std::string objDefName);

		bool AddObjectDefinition(const std::string &objDefName, const XMLNode &xObjectDef);

		// Return the XML associated with an object definition
		XMLNode* FindObjectDefinition(const std::string &objDefName);

		// Can use this to iterate through the object definitions
		int GetObjectDefinitionCount() const;
		const std::string& GetObjectDefinition(int iIndex) const;

		//! Load all object definitions from root <objectDefinitions> node
		bool LoadObjectDefsFromXML(XMLNode &xObjDefs);

		bool LoadObjectDefsFromIncludeXML(std::string file);

		~ObjectFactory();
};

#define OBJECT_FACTORY ObjectFactory::GetInstance()

void RegisterObjectPrototypes();

#endif // OBJECT_FACTORY_H
