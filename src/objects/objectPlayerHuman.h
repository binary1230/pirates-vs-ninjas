#ifndef PLAYEROBJECTHUMAN_H
#define PLAYEROBJECTHUMAN_H

#include "stdafx.h"
#include "object.h"
#include "objectPlayer.h"

class HumanPlayerObject : public PlayerObject {
	protected:
		virtual bool GetInput(uint key, uint controller_number) const;

	public:
		HumanPlayerObject();
		virtual ~HumanPlayerObject();

		friend class ObjectFactory;
};

#endif // PLAYEROBJECTHUMAN_H
