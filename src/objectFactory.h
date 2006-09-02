#ifndef OBJECT_FACTORY_H
#define OBJECT_FACTORY_H

#include <allegro.h>
#include <stdio.h>
#include <vector>

using namespace std;

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

#include "gameBase.h"
#include "xmlParser.h"
#include "StdString.h"
#include "animation.h"
#include "objectIDs.h"

//! A class which creates Object classes from integer ID's
class ObjectFactory : GameBase {
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

		PhysSimulation *physSimulation;

	public:
		int Init(GameState* _game_state);
		void Shutdown();
		
		inline void SetPhysSimulation(PhysSimulation* p) {physSimulation = p;};
		inline PhysSimulation* GetPhysSimulation() {return physSimulation;};

		// Create an object from an XML node
		Object* CreateObjectFromXML(	XMLNode &xObjectDef, XMLNode &xObject);

		// Create an object from an ObjectID
		Object* CreateObject(OBJECTID object_id);

		ObjectFactory();
		~ObjectFactory();
};

#endif // OBJECT_FACTORY_H
