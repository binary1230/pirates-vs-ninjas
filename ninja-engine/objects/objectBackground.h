#ifndef BackgroundObject_H
#define BackgroundObject_H

#include "object.h"

class GameState;

//! An Object which repeats itself, usually used for backgrounds
class ObjectBackground : public Object {
	protected:
		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive &ar, const unsigned int version)
		{
			ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Object);
		}
				
	public:
		IMPLEMENT_CLONE(ObjectBackground)

		bool Init();
		void Shutdown();

		void Update();
		void Draw();

		ObjectBackground();
		~ObjectBackground();

		
};

#endif // BackgroundObject_H   
