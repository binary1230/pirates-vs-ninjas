#ifndef PLAYEROBJECTHUMAN_H
#define PLAYEROBJECTHUMAN_H

#include "object.h"
#include "objectPlayer.h"

class HumanPlayerObject : public PlayerObject {
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Object);
		// ar & BOOST_SERIALIZATION_NVP(a_var_you_want_to_serialize);
	}

	protected:

	public:
		HumanPlayerObject();
		virtual ~HumanPlayerObject();

		virtual bool GetInput(uint key, uint controller_number) const;

		friend class ObjectFactory;
};

#endif // PLAYEROBJECTHUMAN_H
