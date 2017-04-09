#ifndef OBJECT_BRIDGE_H
#define OBJECT_BRIDGE_H

#include "object.h"

class BridgeObject : public Object
{
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive &ar, const unsigned int version)
	{
		ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Object);
		// ar & BOOST_SERIALIZATION_NVP(a_var_you_want_to_serialize);
	}

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