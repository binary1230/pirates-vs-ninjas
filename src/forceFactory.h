#ifndef FORCE_FACTORY_H
#define FORCE_FACTORY_H

#include "stdafx.h"

class Force;
class GameState;

#include "force.h"

//! A class which produces new Force objects from ID's
class ForceFactory {
	protected:

	public:
		int Init();
		void Shutdown();

		Force* CreateForce(ForceType type);
		void DeleteForce(Force*);
		
		ForceFactory();
		~ForceFactory();
};

#endif // FORCE_FACTORY_H
