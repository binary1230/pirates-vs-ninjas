#ifndef OBJECT_FACTORY_H
#define OBJECT_FACTORY_H

#include <allegro.h>
#include <stdio.h>

#include "object.h"
#include "objectIDs.h"

class ObjectFactory {
	protected:

	public:
		Object* CreateObject(uint id);
		void DeleteObject(Object*);

		ObjectFactory();
		~ObjectFactory();
};

#endif // OBJECT_FACTORY_H
