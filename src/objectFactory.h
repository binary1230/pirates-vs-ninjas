#ifndef OBJECT_FACTORY_H
#define OBJECT_FACTORY_H

#include <allegro.h>
#include <stdio.h>

#include "globals.h"
#include "object.h"
#include "objectIDs.h"

#include "objectRadiusBlock.h"
#include "objectMouseBlock.h"

class ObjectFactory {
	protected:
		BITMAP* default_destination_bitmap;

	public:
		int Init();
		void Shutdown();

		Object* CreateObject(uint id);
		void DeleteObject(Object*);
	  
		void SetDefaultDestinationBitmap(BITMAP* bmp);

		ObjectFactory();
		~ObjectFactory();
};

#endif // OBJECT_FACTORY_H
