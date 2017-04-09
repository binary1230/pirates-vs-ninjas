#ifndef BackgroundObject_H
#define BackgroundObject_H

#include "object.h"

class BackgroundObject;
class GameState;
class ObjectFactory;

//! An Object which repeats itself, usually used for backgrounds
class BackgroundObject : public Object {
	protected:
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive &ar, const unsigned int version)
		{
			ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Object);
			// ar & BOOST_SERIALIZATION_NVP(a_var_you_want_to_serialize);
		}
				
	public:
		bool Init();
		void Shutdown();

		void Update();
		void Draw();

		BackgroundObject();
		~BackgroundObject();

		friend class ObjectFactory;
};

#endif // BackgroundObject_H   
