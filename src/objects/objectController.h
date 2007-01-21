#ifndef OBJ_CONTROLLER_H
#define OBJ_CONTROLLER_H

#include "stdafx.h"
#include "object.h"

class Sprite;
class ObjectFactory;

//! A button on the Controller and its associated sprite
struct Button {
	bool active;
	Sprite* sprite;
};

//! A drawable on-screen joystick Object that is shown during demo playback
class ObjectController : public Object {
	protected:
		Sprite* controller_sprite;
		vector<struct Button> buttons;

		bool only_show_during_demo;
		
	public:
		bool Init();	
		void Shutdown();

		void Update();
		void Draw();

		int GetWidth();		// need to override, default ones grab the animation
		int GetHeight();

		ObjectController();
		~ObjectController();

		friend class ObjectFactory;
};

#endif // OBJ_CONTROLLER_H
