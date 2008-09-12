#ifndef OBJECT_BRIDGE_H
#define OBJECT_BRIDGE_H

#include "object.h"

class BridgeObject : public Object
{
	protected:

	public:
		bool Init();
		void Shutdown();

		void Update();
		void Draw();

		BridgeObject();
		~BridgeObject();

		friend class ObjectFactory;
};

#endif // OBJECT_BRIDGE_H