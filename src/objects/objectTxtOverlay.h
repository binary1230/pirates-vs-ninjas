#ifndef OBJ_TXTOVERLAY_H
#define OBJ_TXTOVERLAY_H

#include "stdafx.h"
#include "object.h"

class Sprite;
class ObjectFactory;
class PhysSimulation;

//! A drawable on-screen joystick Object that is shown during demo playback
class ObjectText : public Object {
	protected:
		Sprite* avatar_sprite;
		CString text;
		
	public:
		bool Init(PhysSimulation *p);
		
		void Shutdown();

		void Update();
		void Draw();

		int GetWidth();		// need to override, default ones grab the animation
		int GetHeight();

		ObjectText();
		~ObjectText();

		friend class ObjectFactory;
};

#endif // OBJ_TXTOVERLAY_H
