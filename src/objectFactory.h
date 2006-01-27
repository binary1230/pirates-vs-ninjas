#ifndef OBJECT_FACTORY_H
#define OBJECT_FACTORY_H

#include <allegro.h>
#include <stdio.h>
#include <vector>

using namespace std;

class ObjectFactory;
class Object;
class ResourceLoader;

#include "gameBase.h"
#include "xmlParser.h"
#include "StdString.h"

//! A class which creates Object classes from integer ID's
class ObjectFactory : GameBase {
	protected:
		Object* CreatePlayerObject();
		Object* CreateBackgroundObject();
		Object* CreateRadiusBlockObject();
		
	public:
		int Init(GameState* _game_state);
		void Shutdown();

		// Object* CreateObject(uint id);
		// Object* CreateObject(XMLNode type);
		Object* CreateObject(XMLNode &xObjectDef, XMLNode &xObject);
		void DeleteObject(Object*);

		int LoadObjectsFromXML(XMLNode, vector<Object*> &);
		
		ObjectFactory();
		~ObjectFactory();
};

#endif // OBJECT_FACTORY_H
