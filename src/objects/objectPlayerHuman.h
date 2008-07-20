#ifndef PLAYEROBJECTHUMAN_H
#define PLAYEROBJECTHUMAN_H

#include "object.h"
#include "objectPlayer.h"

class HumanPlayerObject : public PlayerObject {
	protected:

	public:
		HumanPlayerObject();
		virtual ~HumanPlayerObject();

		virtual bool GetInput(uint key, uint controller_number) const;

		friend class ObjectFactory;
};

#endif // PLAYEROBJECTHUMAN_H
