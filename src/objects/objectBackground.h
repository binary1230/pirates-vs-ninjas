#ifndef BackgroundObject_H
#define BackgroundObject_H

#include "object.h"

class BackgroundObject;
class GameState;
class ObjectFactory;

//! An Object which repeats itself, usually used for backgrounds
class BackgroundObject : public Object {
	protected:
				
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
